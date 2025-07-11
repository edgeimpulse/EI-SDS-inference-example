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

#ifndef PLAY_REC_MANAGEMENT_H_
#define PLAY_REC_MANAGEMENT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef  __cplusplus
extern "C"
{
#endif

#include "sds_rec_play.h"

// Assert macro
#ifndef NDEBUG
// SDS error information structure
typedef struct {
  uint8_t     occurred;
  uint8_t     reported;
  const char *file;
  uint32_t    line;
} sdsError_t;

// SDS error information
extern sdsError_t       sdsError;

#define SDS_ASSERT(cond)        if (((cond) == false) && (sdsError.occurred == 0U)) { sdsError.occurred = 1U; sdsError.file = __FILE__; sdsError.line = __LINE__; }
#else
#define SDS_ASSERT(cond)
#endif

// Player/Recorder active status
extern volatile uint8_t playRecActive;

// User request for Player/Recorder to stop
extern volatile uint8_t playRecStop;

// Player/Recorder identifiers
extern sdsRecPlayId_t   playIdModelInput;
extern sdsRecPlayId_t   recIdModelOutput;

// Player/Recorder management thread function
extern void threadPlayRecManagement (void *argument);

#ifdef  __cplusplus
}
#endif

#endif