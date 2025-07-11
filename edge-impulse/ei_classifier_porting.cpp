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

/* Includes */
#include "ei_classifier_porting.h"

#include <stdarg.h>
#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <math.h>

#include "RTE_Components.h"

#ifdef   RTE_CMSIS_RTOS2

#include "cmsis_os2.h"

#else

static uint32_t volatile msTicks; /* counts 1ms timeTicks */

void SysTick_Handler(void)
{
    msTicks++;
}

#endif

EI_IMPULSE_ERROR ei_run_impulse_check_canceled() 
{
    return EI_IMPULSE_OK;
}

/**
 * @brief sleep for the given time in ms
 * 
 * @param time_ms time in ms
 * 
 */
EI_IMPULSE_ERROR ei_sleep(int32_t time_ms)
{
#ifdef RTE_CMSIS_RTOS2
    (void)osDelay((uint32_t)((((uint64_t)time_ms) * osKernelGetTickFreq())/1000UL));
#else
    uint64_t start_time = ei_read_timer_ms();

    while ((start_time + time_ms) > ei_read_timer_ms()){};
#endif

    return EI_IMPULSE_OK;
}

/**
 * @brief Read the time in ms
 * 
 * @return uint64_t time in ms
 */
uint64_t ei_read_timer_ms()
{
#ifdef RTE_CMSIS_RTOS2
    return ((((uint64_t)osKernelGetTickCount()) * 1000ULL) / osKernelGetTickFreq());
#else
    return (uint64_t)msTicks;
#endif
}

/**
 * @brief Read the time in us
 * 
 * 
 * @return uint64_t time in us
 */
uint64_t ei_read_timer_us()
{
#ifdef RTE_CMSIS_RTOS2
    return (((uint64_t)osKernelGetTickCount()) * (1000000UL / osKernelGetTickFreq()));
#else
    return (uint64_t)(msTicks * 1000);
#endif
}

/**
 * @brief Read a character from the standard input.
 * 
 */
char ei_getchar()
{
    return getchar();
}

/**
 * @brief Wrapper for printf function. The string is to be printed on the console. 
 * 
 * @note This function is weakly linked and can be overridden by the user.
 * 
 */
void ei_printf(const char *format, ...) 
{
    va_list myargs;
    va_start(myargs, format);
    vprintf(format, myargs);
    va_end(myargs);
}

/**
 * @brief Print a float on console. Needed if printf of float number is not supported by the platform
 * 
 * @note This function is weakly linked and can be overridden by the user.
 * 
 * @param f float number to be printed
 */
void ei_printf_float(float f)
{
    ei_printf("%f", f);
}

/**
 * @brief Print char on console
 * 
 * @param c 
 */
void ei_putchar(char c)
{
    putchar(c);
}

/**
 * @brief malloc wrapper
 * 
 */
void *ei_malloc(size_t size)
{
    return malloc(size);
}

/**
 * @brief calloc wrapper
 * 
 */
void *ei_calloc(size_t nitems, size_t size)
{
    return calloc(nitems, size);
}

/**
 * @brief free wrapper
 * 
 */
void ei_free(void *ptr)
{
    free(ptr);
}
