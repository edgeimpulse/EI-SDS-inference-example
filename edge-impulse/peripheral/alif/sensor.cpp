/* The Clear BSD License
 *
 * Copyright (c) 2025 EdgeImpulse Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted (subject to the limitations in the disclaimer
 * below) provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 *   * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 *   * Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
 * THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensor.h"
#include <string.h>
#include "../../ei_main.h"
#include <stdio.h>

#if USE_OLD_IMU == 1
#include "inertial/bmi323_icm42670.h"
#else
#include "Driver_IMU.h"
/* IMU Driver instance */
extern ARM_DRIVER_IMU BMI323;
static ARM_DRIVER_IMU *Drv_IMU = &BMI323;

#endif


#define INERTIAL_AXIS_SAMPLED   6
/* Constant defines -------------------------------------------------------- */
#define CONVERT_G_TO_MS2 (9.80665f)
#define CONVERT_mG_TO_MS2 (9.80665f/1000.0f)
#define ACC_RAW_SCALING  (32767.5f)

#define ACC_SCALE_FACTOR (16.0f * CONVERT_G_TO_MS2) / ACC_RAW_SCALING
#define CONVERT_ADC_GYR (float)(250.0f / 32768.0f)

/* Private variables ------------------------------------------------------- */
//static float imu_data[INERTIAL_AXIS_SAMPLED];
static float imu_data_bmi323[INERTIAL_AXIS_SAMPLED];

static void ei_fusion_inertial_read_data_icm42670(int n_samples);
static void ei_fusion_inertial_read_data_bmi323(int n_samples);

// Event flags
//osEventFlagsId_t sensor_event = NULL;

static bool imu_init = false;

/**
 * @brief Initialize the inertial sensor
 */
bool ei_inertial_init(void)
{
    int32_t ret;

#if USE_OLD_IMU == 1
    if (bmi323_icm42670_init() != 0) {
        printf("\r\n Error: BMI323/ICM42670 initialization failed.\r\n");        
    }
#else
    /* IMU initialization */
    ret = Drv_IMU->Initialize();
    if(ret != ARM_DRIVER_OK)
    {
        printf("\r\n Error: IMU Initialize failed.\r\n");
        return false;
    }

    /* IMU power up */
    ret = Drv_IMU->PowerControl(ARM_POWER_FULL);
    if(ret != ARM_DRIVER_OK)
    {
        printf("\r\n Error: IMU Power-up failed.\r\n");
        return false;
    }
#endif
    imu_init = true;
    return true;
}

/**
 * 
 */
static void ei_fusion_inertial_read_data_bmi323(int n_samples)
{

    int32_t ret;

    memset(imu_data_bmi323, 0, sizeof(imu_data_bmi323));

#if USE_OLD_IMU == 1
    xyz_accel_gyro_imu_s bmi_data;

    getBMI323AllSensors(&bmi_data);

    imu_data_bmi323[0] = (float)bmi_data.acc_x * ACC_SCALE_FACTOR;
    imu_data_bmi323[1] = (float)bmi_data.acc_y * ACC_SCALE_FACTOR;
    imu_data_bmi323[2] = (float)bmi_data.acc_z * ACC_SCALE_FACTOR;
#else
    ARM_IMU_COORDINATES data;
    ARM_IMU_STATUS      status;
    float               temperature;

    Drv_IMU->Control(IMU_SET_INTERRUPT, true);  // mandatory ?

    /* Gets IMU status */
    do {
        status = Drv_IMU->GetStatus();

        if(status.drdy_status & IMU_ACCELEROMETER_DATA_READY) {
            /* Read Accelerometer data */
            ret = Drv_IMU->Control(IMU_GET_ACCELEROMETER_DATA,
                                    (uint32_t)&data);
            imu_data_bmi323[0] = (float)data.x * ACC_SCALE_FACTOR;
            imu_data_bmi323[1] = (float)data.y * ACC_SCALE_FACTOR;
            imu_data_bmi323[2] = (float)data.z * ACC_SCALE_FACTOR;
            break;
            }
        else {
            osDelay(1);
        }
    }while(1);

#endif
}

__NO_RETURN void sensorThread (void *argument)
{   
    (void)argument; 
    uint32_t interval_time = 0U;
    for(;;) {
        interval_time = osKernelGetTickCount(); // get time
        ei_fusion_inertial_read_data_bmi323(3);

        samples_callback(imu_data_bmi323, 3 * sizeof(float));
      
        interval_time += EI_CLASSIFIER_INTERVAL_MS;
        osDelayUntil(interval_time);
    }
}
