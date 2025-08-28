/*
 * Copyright (c) 2025 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sensor_vstream.h"

#include <stdio.h>
#include <string.h>

#include "app_main.h"
#include "ei_main.h"

#include "cmsis_os2.h"
#include "cmsis_compiler.h"

#include "vstream_accelerometer.h"
#include "model-parameters/model_metadata.h"

// Configuration

#ifndef SENSOR_ITEMS_PER_SAMPLE
#define SENSOR_ITEMS_PER_SAMPLE         (3U)    // For accelerometer there are x, y, z (3) items
#endif

#ifndef SENSOR_BYTES_PER_SAMPLE
#define SENSOR_BYTES_PER_SAMPLE         (SENSOR_ITEMS_PER_SAMPLE * 2u)    // For accelerometer there are 3 items of 2 bytes per sample
#endif

#ifndef SENSOR_SAMPLES_PER_SLICE
#define SENSOR_SAMPLES_PER_SLICE        (1)   // Number of samples per inference slice
#endif

// Number of bytes per inference slice
#define SENSOR_SLICE_SIZE_IN_BYTES      (SENSOR_SAMPLES_PER_SLICE * SENSOR_BYTES_PER_SAMPLE)

// Number of items per inference slice
#define SENSOR_SLICE_SIZE_IN_ITEMS      (SENSOR_SAMPLES_PER_SLICE * SENSOR_ITEMS_PER_SAMPLE)

// Timestamp of read of input data for inference
uint32_t rec_timestamp;

// Pointer to vStream driver
static vStreamDriver_t *ptrDriver_vStreamAccelerometer = &Driver_vStreamAccelerometer;

// Raw sensor data sample structure
typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} accelerometer_sample_t;

// Raw sensor data (2 * slice)
static uint8_t vstream_buf[SENSOR_SLICE_SIZE_IN_BYTES*2] __ALIGNED(4);

// ML input data (1 slice of items in float format)
static float   scaled_sensor_data[SENSOR_SLICE_SIZE_IN_ITEMS];

// Function that sends event when data is available with vStream
static void vStreamSensorEvent (uint32_t event_flags) {

    if ((event_flags & VSTREAM_EVENT_DATA) != 0U) {
    // Inform main ML thread that sensor data is ready
        osThreadFlagsSet(thrId_threadSensor, SENSOR_DATA_READY_FLAG);
    }
    if ((event_flags & VSTREAM_EVENT_OVERFLOW) != 0U) {
        printf("Warning: Accelerometer overflow event!\r\n");
    }

}

// Thread function that activates sensor and does polling of sensor data
__NO_RETURN void sensorThread (void *argument) {
  accelerometer_sample_t *ptr_acc_sample;
  float                  *ptr_scaled_sensor_data;
  uint32_t interval_time = 0U;
  (void)argument;

  ptrDriver_vStreamAccelerometer->Initialize(vStreamSensorEvent);
  ptrDriver_vStreamAccelerometer->SetBuf(&vstream_buf, sizeof(vstream_buf), SENSOR_SLICE_SIZE_IN_BYTES);
  ptrDriver_vStreamAccelerometer->Start(VSTREAM_MODE_CONTINUOUS);

  for (;;) {

    interval_time = osKernelGetTickCount(); // get time
    uint32_t flags = osThreadFlagsWait(SENSOR_DATA_READY_FLAG, osFlagsWaitAny, osWaitForever);

    if (((flags & osFlagsError)           == 0U) &&         // If not an error and
        ((flags & SENSOR_DATA_READY_FLAG) != 0U)) {         // if flag is sensor data ready

        // Get pointer to sensor data
        ptr_acc_sample = (accelerometer_sample_t *)ptrDriver_vStreamAccelerometer->GetBlock();

        // Set pointer for scaled sensor data
        ptr_scaled_sensor_data = scaled_sensor_data;


        // Convert each sample value for x, y, z from int16 to float scaled to range used during model training
        ptr_scaled_sensor_data[0]  = ((float)ptr_acc_sample->x) / 1638.4f;
        ptr_scaled_sensor_data[1]  = ((float)ptr_acc_sample->y) / 1638.4f;
        ptr_scaled_sensor_data[2]  = ((float)ptr_acc_sample->z) / 1638.4f;
        //ptr_acc_sample            += 1U;
        //ptr_scaled_sensor_data    += SENSOR_ITEMS_PER_SAMPLE;

        // Release data block
        ptrDriver_vStreamAccelerometer->ReleaseBlock();

        // Inform main ML thread that sensor data is ready
        samples_callback(ptr_scaled_sensor_data, SENSOR_SLICE_SIZE_IN_ITEMS * sizeof(float));

        // Wait for time interval simulating real world sensor data acquisition
        interval_time += EI_CLASSIFIER_INTERVAL_MS;
        osDelayUntil(interval_time);
    }
  }
}
