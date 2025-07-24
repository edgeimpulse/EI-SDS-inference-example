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

#ifndef SENSOR_VSTREAM_H_
#define SENSOR_VSTREAM_H_

#include <stddef.h>
#include <stdint.h>

// Thread flag for signaling sensor data available
#define SENSOR_DATA_READY_FLAG              1U

#ifdef  __cplusplus
extern "C"
{
#endif

// Timestamp of read of input data for inference
extern uint32_t rec_timestamp;

// Get sensor data for inference
extern int sensor_get_data (size_t offset, size_t length, float *out_ptr);

// Thread function that activates sensor and does polling of sensor data
extern void sensorThread (void *argument);

#ifdef  __cplusplus
}
#endif

#endif