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
#include <ei_main.h>

#include "ei_run_classifier.h"
#include "edge-impulse-sdk/porting/ei_classifier_porting.h"
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "model-parameters/model_variables.h"
#include "cmsis_os2.h"
#include "play_rec_management.h"

// Timestamp of read of input data for inference
static uint32_t timestamp;

/**
 * @brief      Get raw features from the SDS player
 */
int raw_feature_get_data(size_t offset, size_t length, float *out_ptr)
{
#if SDS_PLAY && (SDS_PLAY == 1)

    // Playback previously recorded data from ModelInput.0.sds file
    uint32_t num = sdsPlayRead(playIdModelInput, &timestamp, (void *)out_ptr, (length * sizeof(float)));

    if (num != (length * sizeof(float))) {
      // Not enough data read, probably end of playback
      // Clear last buffer for inference to allow inference to execute
      memset((void *)out_ptr, 0, (length * sizeof(float)));
    }

    if (sdsPlayEndOfStream(playIdModelInput) != 0) {
      // No more playback data available

      // Stop Playback/Recording
      playRecActive = 0U;
      playRecStop   = 1U;
    }
#else
    // collect real data

#endif

    return 0;
}

/**
 * @brief      Get the raw features from the sensor and run the classifier
 */
extern "C" int ei_main(void)
{
    ei_impulse_result_t result = {nullptr};

    signal_t features_signal;
    features_signal.total_length = EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE;
    features_signal.get_data = &raw_feature_get_data;

    while (1) {

        if (playRecActive != 0U) {

            // invoke the impulse
            EI_IMPULSE_ERROR res = run_classifier(&features_signal, &result, false);

            if (res != 0) {
                ei_printf("ERR: Failed to run classifier\n");
                return 1;
            }

            display_results(&ei_default_impulse, &result);

            // Output, for example:
            // idle: 0.996094
            // snake: 0.000000
            // updown: 0.000000
            // wave: 0.000000

            // If playback/recording is active then record model output data
            // Prepare model output data for recording
            float model_out_results[EI_CLASSIFIER_NN_OUTPUT_COUNT];

            for (uint8_t i = 0U; i < EI_CLASSIFIER_NN_OUTPUT_COUNT; i++) {
                model_out_results[i] = result.classification[i].value;
            }

            // Record model output data
            uint32_t num = sdsRecWrite(recIdModelOutput, timestamp, model_out_results, EI_CLASSIFIER_NN_OUTPUT_COUNT * sizeof(float));
            SDS_ASSERT(num == (EI_CLASSIFIER_NN_OUTPUT_COUNT * sizeof(float)));
        }

        ei_sleep(2000);
    }

    run_classifier_deinit();

    return 0;
}

void ei_init(void)
{
    ei_printf("Edge Impulse inferencing\n");

    // summary of inferencing settings (from model_metadata.h)
    ei_printf("Inferencing settings:\n");
    ei_printf("\tClassifier interval: %.2f ms.\n", (float)EI_CLASSIFIER_INTERVAL_MS);
    ei_printf("\tInput frame size: %d\n", EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE);
    ei_printf("\tRaw sample count: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLE_COUNT);
    ei_printf("\tRaw samples per frame: %d samples.\n", EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME);
    ei_printf("\tNumber of output classes: %d\n", sizeof(ei_classifier_inferencing_categories) / sizeof(ei_classifier_inferencing_categories[0]));

    // init sensor
#if SDS_PLAY && (SDS_PLAY == 1)
    //
#else
    //
#endif

    run_classifier_init();    
}
