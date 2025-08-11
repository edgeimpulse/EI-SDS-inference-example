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
#ifndef SIMULATOR
#ifdef  RTE_SDS_IO_SOCKET
#include "sdsio_config_socket.h"
#else

#endif
#endif
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
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
sdsRecPlayId_t playIdModelInput = NULL;
#else
sdsRecPlayId_t recIdDataInput  = NULL;
#endif
sdsRecPlayId_t recIdModelOutput = NULL;

// SDS Player/Recorder buffers
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
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

#ifdef SIMULATOR
// Simulate keypress
static uint32_t simGetSignal (uint32_t mask) {
  static uint32_t key_cnt = 0U;
         uint32_t ret     = 0U;

  switch (key_cnt) {
    case 20U:                           // At 2 seconds
      ret = mask;                       // Simulate keypress
      break;
#ifndef SDS_PLAY
    case 120U:                          // At 12 seconds
      ret = mask;                       // Simulate keypress
      break;
#endif
    case 150U:                          // At 15 seconds
      putchar(0x04);                    // Send signal to simulator to shutdown
      break;
  }

  key_cnt++;

  return ret;
}
#endif

// Playback/Recording control thread function.
__NO_RETURN void threadPlayRecManagement (void *argument) {
    int32_t status;
    uint8_t btn_val, keypress;
    uint8_t btn_prev = 0U;
    uint8_t led0_val = 0U;
    uint32_t no_load_cnt, prev_cnt;
    uint32_t interval_time, cnt = 0U;

//#ifdef RTE_SDS_IO_SOCKET
//    // Initialize socket interface
//    status = socket_startup();
//    SDS_ASSERT(status == 0);
//#endif

#ifndef SIMULATOR
#ifdef RTE_SDS_IO_SOCKET
    printf("Tryint to connect to SDS server %s\n", SDSIO_SOCKET_SERVER_IP);
#elif defined(RTE_SDS_IO_USB)
    printf("Tryint to connect to SDS USB interface\n");
#else

#endif

#endif

    // Initialize SDS recorder/player
    status = sdsRecPlayInit(recorder_event_callback);
    SDS_ASSERT(status == SDS_REC_PLAY_OK);

    if (status != SDS_REC_PLAY_OK) {
        printf("Failed to initialize SDS recorder/player!\n");
        vioSetSignal(vioLED0, vioLEDon);
        while(1) {
            (void)osDelay(1u);
        }
        
    }


#ifndef SIMULATOR
#ifdef RTE_SDS_IO_SOCKET
    printf("Connected to %s\n", SDSIO_SOCKET_SERVER_IP);
#elif defined(RTE_SDS_IO_USB)
    printf("Connected to SDS USB interface\n");
#else

#endif
#endif

    interval_time = osKernelGetTickCount();

    for (;;) {     
        
#ifdef SIMULATOR
        btn_val  = simGetSignal(vioBUTTON0);
#else
        btn_val  = vioGetSignal(vioBUTTON0);
#endif
        keypress = btn_val & ~btn_prev;
        btn_prev = btn_val;

        switch (sdsStreamingState) {
            case SDS_STREAMING_INACTIVE:
            if (!keypress) break;
                printf("SDS: Start streaming\n");
                if (OpenStreams() == 0) {
                    // Turn LED1 on
                    vioSetSignal(vioLED1, vioLEDon);
                    sdsStreamingState = SDS_STREAMING_ACTIVE;
                    ei_start_impulse();  // command to start inference
                }                
            break;
            case SDS_STREAMING_ACTIVE:
                if (!keypress) break;
                printf("SDS: Stop streaming\n");
                // Request to stop streaming
                sdsStreamingState = SDS_STREAMING_STOP;
                ei_stop_impulse();  // command to stop inference
            break;
            case SDS_STREAMING_STOP_SAFE:
                printf("SDS: SDS_STREAMING_STOP_SAFE\n");
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
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
    // Start playback of previously recorded Model Input data
    playIdModelInput = sdsPlayOpen("ModelRec",
                                    sds_play_buf_model_in,
                                    sizeof(sds_play_buf_model_in));
    if (playIdModelInput == NULL) {
        printf("Failed to open SDS stream for playback!\n");
        return -1;
    }
#else
    // Start playback of previously recorded Model Input data
    recIdDataInput = sdsRecOpen("ModelRec",
                                    sds_rec_buf_model_in,
                                    sizeof(sds_rec_buf_model_in));
    if (recIdDataInput == NULL) {
        printf("Failed to open SDS stream for recording data input!\n");
        return -1;
    }
#endif

    // Start recording of Model Output data
    recIdModelOutput = sdsRecOpen("ModelOutput",
                                    sds_rec_buf_model_out,
                                    sizeof(sds_rec_buf_model_out));                                  
    if (recIdModelOutput == NULL) {
        printf("Failed to open SDS stream for recording model output!\n");
        return -1;
    }

    return 0;
}

/**
 * @brief      Close SDS streams
 */
static int32_t CloseStreams (void)
{
    int32_t status = 0;

#if (defined SDS_PLAY) && (SDS_PLAY == 1)
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
