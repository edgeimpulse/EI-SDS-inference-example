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

#include "play_rec_management.h"
#include "RTE_Components.h"
#include "cmsis_os2.h"
#include "cmsis_vio.h"
#include "ei_main.h"
#include <stdio.h>

// Configuration

// SDS Player/Recorder data buffers size
#ifndef PLAY_BUF_SIZE_MODEL_IN
#define PLAY_BUF_SIZE_MODEL_IN          8192U
#endif
#ifndef REC_BUF_SIZE_MODEL_OUT
#define REC_BUF_SIZE_MODEL_OUT          576U
#endif

#ifdef  RTE_SDS_IO_SOCKET
extern  int32_t  socket_startup (void);
#endif

// SDS error information
#ifndef NDEBUG
sdsError_t       sdsError = { 0U, 0U, NULL, 0U };
#endif

// SDS streams status
volatile uint8_t sdsStreamingState = SDS_STREAMING_INACTIVE;

// SDS stream identifiers
#ifdef SDS_PLAY
sdsRecPlayId_t playIdDataInput = NULL;
#else
static sdsRecPlayId_t recIdDataInput  = NULL;
#endif
static sdsRecPlayId_t recIdDataOutput = NULL;

// SDS Player/Recorder buffers
#ifdef SDS_PLAY
static uint8_t   sds_play_buf_model_in[PLAY_BUF_SIZE_MODEL_IN];
#else
static uint8_t   sds_rec_buf_model_in[PLAY_BUF_SIZE_MODEL_IN];
#endif
static uint8_t   sds_rec_buf_model_out[REC_BUF_SIZE_MODEL_OUT];

static int32_t OpenStreams (void);
static int32_t CloseStreams (void);

// Recorder/Player event callback
static void recorder_event_callback (sdsRecPlayId_t id, uint32_t event) {
    if ((event & SDS_REC_PLAY_EVENT_ERROR_IO) != 0U) {
        SDS_ASSERT(false);
    }
    if ((event & SDS_REC_EVENT_ERROR_NO_SPACE) != 0U) {
        SDS_ASSERT(false);
    }
    if ((event & SDS_PLAY_EVENT_ERROR_NO_DATA) != 0U) {
        SDS_ASSERT(false);
    }
}

// Playback/Recording control thread function.
__NO_RETURN void threadPlayRecManagement (void *argument) {
    int32_t status;
    uint8_t btn_val, keypress;
    uint8_t btn_prev = 0U;
    uint8_t led0_val = 0U;
    uint32_t no_load_cnt, prev_cnt;
    uint32_t interval_time, cnt = 0U;

#ifdef RTE_SDS_IO_SOCKET
    // Initialize socket interface
    status = socket_startup();
    SDS_ASSERT(status == 0);
#endif

    // Initialize SDS recorder/player
    status = sdsRecPlayInit(recorder_event_callback);
    SDS_ASSERT(status == SDS_REC_PLAY_OK);


    interval_time = osKernelGetTickCount();

    for (;;) {     
        
        btn_val  = vioGetSignal(vioBUTTON0);
        keypress = btn_val & ~btn_prev;
        btn_prev = btn_val;

        switch (sdsStreamingState) {
            case SDS_STREAMING_INACTIVE:
            if (!keypress) break;
                if (OpenStreams() == 0) {
                    // Turn LED1 on
                    vioSetSignal(vioLED1, vioLEDon);
                    sdsStreamingState = SDS_STREAMING_ACTIVE;
                    ei_start_impulse();  // command to start inference
                }                
            break;
            case SDS_STREAMING_ACTIVE:
                if (!keypress) break;

                // Request to stop streaming
                sdsStreamingState = SDS_STREAMING_STOP;
                ei_stop_impulse();  // command to stop inference
            break;
            case SDS_STREAMING_STOP_SAFE:
                if (CloseStreams() == 0) {
                    // Turn LED1 off
                    vioSetSignal(vioLED1, vioLEDoff);
                    sdsStreamingState = SDS_STREAMING_INACTIVE;
                }
            break;
            case SDS_STREAMING_STOP:
                // wait till we receive SDS_STREAMING_STOP_SAFE
            break;
            default:
            break;
        }

        interval_time += 100U;
        osDelayUntil(interval_time);    // debounce

        (void)osDelay(100U);

        // Do 1 second interval
        if (++cnt == 10U) {
            cnt = 0U;

            // Toggle LED0
            led0_val ^= 1U;
            vioSetSignal(vioLED0, led0_val);
        }
    }
}

uint8_t get_sdsStreamingState(void)
{
    return sdsStreamingState;
}

void set_sdsClosed(void)
{
    if (SDS_STREAMING_STOP == sdsStreamingState) {
        sdsStreamingState = SDS_STREAMING_STOP_SAFE;
    }
}

/**
 * @brief      Open SDS streams for playback or recording
 */
static int32_t OpenStreams (void)
{
    int32_t status = 0;

#if SDS_PLAY && (SDS_PLAY == 1)
    // Start playback of previously recorded Model Input data
    playIdModelInput = sdsPlayOpen("ModelInput",
                                    sds_play_buf_model_in,
                                    sizeof(sds_play_buf_model_in));
    SDS_ASSERT(playIdModelInput != NULL);
#else
    // Start playback of previously recorded Model Input data
    recIdDataInput = sdsPlayOpen("ModelInput",
                                    sds_rec_buf_model_in,
                                    sizeof(sds_rec_buf_model_in));
    SDS_ASSERT(playIdModelInput != NULL);
#endif

    // Start recording of Model Output data
    recIdModelOutput = sdsRecOpen("ModelOutput",
                                    sds_rec_buf_model_out,
                                    sizeof(sds_rec_buf_model_out));                                  
    SDS_ASSERT(recIdModelOutput != NULL);

    return status;
}

/**
 * @brief      Close SDS streams
 */
static int32_t CloseStreams (void)
{
    int32_t status = 0;

#if SDS_PLAY && (SDS_PLAY == 1)
    // Close stream for playback of input data
    status = sdsPlayClose(playIdModelInput);
    SDS_ASSERT(status == SDS_REC_PLAY_OK);
    if (status != 0) {
        printf("Failed to close SDS stream for playback of input data!\n");
        status = -1;
    }
#else
    // Close stream for recording of input data
    status = sdsRecClose(recIdDataInput);
    SDS_ASSERT(status == SDS_REC_PLAY_OK);
    if (status != 0) {
        printf("Failed to close SDS stream for recording of input data!\n");
        status = -1;
    }
#endif

    // Stop recording of Model Output data
    if (recIdModelOutput != NULL) {
        status = sdsRecClose(recIdModelOutput);
        SDS_ASSERT(status == SDS_REC_PLAY_OK);
        recIdModelOutput = NULL;
    }

    return status;
}
