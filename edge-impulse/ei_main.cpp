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

#include <string.h>
#include "ei_main.h"

#include "ei_run_classifier.h"
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_variables.h"

#if (EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_ACCELEROMETER)
#error "This example is configured for accelerometer data only"
#endif

#include <sdsio.h>
#include "play_rec_management.h"
#ifndef SDS_PLAY
#ifdef ALIF_TARGET
#include "peripheral/alif/sensor.h"
#else
#include "peripheral/vstream/sensor_vstream.h"
#endif
#endif

typedef enum {
    INFERENCE_STOPPED,
    INFERENCE_WAITING,
    INFERENCE_SAMPLING,
    INFERENCE_DATA_READY
} inference_state_t;

// Timestamp of read of input data for inference
static uint32_t timestamp;

// Event flags
osEventFlagsId_t inferencing_event = NULL;

static inference_state_t state = INFERENCE_STOPPED;

static uint16_t samples_per_inference;
static float samples_circ_buff[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE]  __ALIGNED(4);
static int samples_wr_index = 0;

static void ei_run_inference(void);
extern sdsRecPlayId_t recIdDataInput;

#define INFERENCING_EVENT_START_FLAG    (1U << 0)
#define INFERENCING_EVENT_STOP_FLAG     (1U << 1)
#define INFERENCING_SENSOR_FULL     (1U << 2)

/**
 * @brief Called for each single sample
 */
bool samples_callback(const void *raw_sample, uint32_t raw_sample_size)
{
    if (state != INFERENCE_SAMPLING) {
        // stop collecting samples if we are not in SAMPLING state
        return false;
    }

    float *sample = (float *)raw_sample;

    for (int i = 0; i < (int)(raw_sample_size / sizeof(float)); i++) {
        samples_circ_buff[samples_wr_index++] = sample[i];
        if (samples_wr_index > EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE) {
            /* start from beginning of the circular buffer */
            samples_wr_index = 0;
        }

        if (samples_wr_index >= samples_per_inference) {
            
            state = INFERENCE_DATA_READY;
            osEventFlagsSet(inferencing_event, INFERENCING_SENSOR_FULL);
            ei_printf("Samples collected: %d\n", samples_wr_index);
            ei_printf("Recording done\n");

                    // Timestamp of read of input data for inference
            uint32_t rec_timestamp;
    
            // Record raw sample data
            rec_timestamp = osKernelGetTickCount();
            uint32_t num  = sdsRecWrite(recIdDataInput, rec_timestamp, samples_circ_buff, sizeof(samples_circ_buff) );
            SDS_ASSERT(num == (sizeof(samples_circ_buff)));

            return true;
        }
    }

    return false;
}

/**
 * @brief      Get raw features from the SDS player
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr)
{
#if (defined SDS_PLAY) && (SDS_PLAY == 1)

    // Playback previously recorded data from ModelInput.0.sds file
    uint32_t num = sdsPlayRead(playIdModelInput, &timestamp, (void *)out_ptr, (length * sizeof(float)));

    if (num != (length * sizeof(float))) {
      // Not enough data read, probably end of playback
      // Clear last buffer for inference to allow inference to execute
      memset((void *)out_ptr, 0, (length * sizeof(float)));
    }

    if (num == SDSIO_EOS) {
        // No more playback data available
        // Stop Playback/Recording
        //playRecActive = 0U;
        //playRecStop   = 1U;
    }

#else
    //
#endif

    return 0;
}

/**
 * @brief      State machine for Edge Impulse inferencing
 */
extern "C" int ei_main(void)
{
    uint32_t flags;
    while (1) {

        switch(state) {
            case INFERENCE_STOPPED:
                set_sdsClosed();
                osEventFlagsClear(inferencing_event, INFERENCING_EVENT_STOP_FLAG);
                flags = osEventFlagsWait(inferencing_event, INFERENCING_EVENT_START_FLAG, osFlagsWaitAny, osWaitForever);
                if ((flags & INFERENCING_EVENT_START_FLAG) != 0U) {                    
                    state = INFERENCE_WAITING;
                    ei_printf("ei_main -> INFERENCE_WAITING\n");
                }
                // nothing to do
            break;
            case INFERENCE_WAITING:
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
                flags = osEventFlagsWait(inferencing_event, INFERENCING_EVENT_STOP_FLAG, osFlagsWaitAny, 0);
                if ((flags & INFERENCING_EVENT_STOP_FLAG) != 0U) {                    
                    //state = INFERENCE_STOPPED;
                    ei_printf("ei_main -> INFERENCE_STOPPED\n");
                    break;
                }
#endif
                ei_printf("wait 1 sec");
                osDelay(1000); // wait for 1 second before checking the event flags again
#ifndef SDS_PLAY
               state = INFERENCE_SAMPLING;
               ei_printf("ei_main -> INFERENCE_SAMPLING\n");
            break;
            case INFERENCE_SAMPLING:
                flags = osEventFlagsWait(inferencing_event, INFERENCING_SENSOR_FULL | INFERENCING_EVENT_STOP_FLAG, osFlagsWaitAny, osWaitForever);
            break;
            case INFERENCE_DATA_READY:
#endif
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
                flags = osEventFlagsWait(inferencing_event, INFERENCING_EVENT_STOP_FLAG, osFlagsWaitAny, 0);
                if ((flags & INFERENCING_EVENT_STOP_FLAG) != 0U) {                    
                    //state = INFERENCE_STOPPED;
                    ei_printf("ei_main -> INFERENCE_STOPPED\n");
                    break;
                }
#endif
                ei_run_inference();
                state = INFERENCE_WAITING;
            break;
                default:
                    break;
            }

        ei_sleep(2000);
    }

    run_classifier_deinit();

    return 0;
}

extern "C" void ei_init(void)
{
    inferencing_event = osEventFlagsNew(NULL);
    SDS_ASSERT(inferencing_event != NULL);

    ei_printf("Edge Impulse inferencing\n");

    ei_printf("Inferencing settings:\n");
    ei_printf("\tClassifier interval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tInput frame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tRaw sample count: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
    ei_printf("\tRaw samples per frame: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);
    ei_printf("\tNumber of output classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    //
}

/**
 * @brief
 */
extern "C" void ei_start_impulse(void)
{
    ei_printf("Start impulse");
    run_classifier_init();
    samples_per_inference = EI_CLASSIFIER_RAW_SAMPLE_COUNT * EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME;
    osEventFlagsSet(inferencing_event, INFERENCING_EVENT_START_FLAG);
}

/**
 * @brief
 */
extern "C" void ei_stop_impulse(void)
{    
    if (state != INFERENCE_STOPPED) {
        state = INFERENCE_STOPPED;
        ei_printf("Inferencing stopped by user\r\n");
        osEventFlagsSet(inferencing_event, INFERENCING_EVENT_STOP_FLAG);
        
        run_classifier_deinit();    // ?
    }
}

/**
 * @brief
 */
extern "C" bool is_inference_running(void)
{
    return (state != INFERENCE_STOPPED);
}


/**
 * @brief      Run the inference
 */
static void ei_run_inference(void)
{
    ei_impulse_result_t result = {nullptr};

    signal_t features_signal;
#if (defined SDS_PLAY) && (SDS_PLAY == 1)
    features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;

    features_signal.get_data = &raw_feature_get_data;
#else
    // shift circular buffer, so the newest data will be the first
    // if samples_wr_index is 0, then roll is immediately returning
    numpy::roll(samples_circ_buff, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, (-samples_wr_index));
    /* reset wr index, the oldest data will be overwritten */
    samples_wr_index = 0;

    // Create a data structure to represent this window of data
    int err = numpy::signal_from_buffer(samples_circ_buff, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &features_signal);
#endif

    // invoke the impulse
    EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);

    if (res != 0) {
        ei_printf("ERR: Failed to run classifier\n");
        return;
    }

    display_results(&ei_default_impulse, &result);

    uint16_t num_element = 0;
    float *model_out_results;

#if (EI_CLASSIFIER_OBJECT_DETECTION == 1)
    model_out_results = (float*)ei_malloc(result.bounding_boxes_count * sizeof(float));

    bool bb_found = result.bounding_boxes[0].value > 0;
    for (size_t ix = 0; ix < result.bounding_boxes_count; ix++) {
        auto bb = result.bounding_boxes[ix];
        if (bb.value == 0) {
            continue;
        }
        model_out_results[num_element++] = bb.value;
    }

#elif (EI_CLASSIFIER_LABEL_COUNT == 1) && (!EI_CLASSIFIER_HAS_ANOMALY)

#elif (EI_CLASSIFIER_LABEL_COUNT > 1)
    num_element = EI_CLASSIFIER_LABEL_COUNT;
    model_out_results = (float*)ei_malloc(num_element * sizeof(float));

    for (uint16_t i = 0U; i < EI_CLASSIFIER_NN_OUTPUT_COUNT; i++) {
        model_out_results[i] = result.classification[i].value;
    }
#endif

    // Record model output data
    uint32_t num = sdsRecWrite(recIdModelOutput, timestamp, model_out_results, num_element * sizeof(float));
    SDS_ASSERT(num == (num_element * sizeof(float)));

    ei_free(model_out_results);
}
