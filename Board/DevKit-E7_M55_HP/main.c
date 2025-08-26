/*---------------------------------------------------------------------------
 * Copyright (c) 2025 Arm Limited (or its affiliates). All rights reserved.
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
 *---------------------------------------------------------------------------*/

#include "RTE_Components.h"
#include CMSIS_device_header
#include "cmsis_vio.h"
#ifdef CMSIS_shield_header
#include CMSIS_shield_header
#endif

#include "board_config.h"
#include "ethosu_driver.h"
#include "main.h"

#include "se_services_port.h"

#ifdef CMSIS_shield_header
__WEAK int32_t shield_setup(void)
{
    return 0;
}
#endif

/* Ethos NPU driver instance. */
static struct ethosu_driver EthosDriver;

/*
  Ethos NPU interrupt handler.
*/
void NPU_HP_IRQHandler(void)
{
    ethosu_irq_handler(&EthosDriver);
}

/*
  Initializes the Ethos NPU driver.
*/
int32_t NpuInit(void)
{
    void *const ethos_base_addr = (void *) NPULOCAL_BASE;

    /*  Initialize Ethos-U NPU driver. */
    if (ethosu_init(&EthosDriver,    /* Ethos-U device driver */
                    ethos_base_addr, /* Ethos-U base address  */
                    0,               /* Cache memory pointer  */
                    0,               /* Cache memory size     */
                    1,               /* Secure enable         */
                    1)               /* Privileged mode       */
    ) {
        /* Failed to initialize Arm Ethos-U driver */
        return 1;
    }

    NVIC_EnableIRQ(NPULOCAL_IRQ_IRQn);

    return 0;
}

/*
  Initializes clocks.
*/
void clock_init(void)
{
    uint32_t rval;
    uint32_t error_code = 0;
    run_profile_t runp = {0};

    /* Enable 100M_CLK */
    rval = SERVICES_clocks_enable_clock(se_services_s_handle, CLKEN_CLK_100M, true, &error_code);
    if ((rval != 0) || (error_code != 0)) {
        return;
    }

    /* Enable HFOSC_CLK */
    rval = SERVICES_clocks_enable_clock(se_services_s_handle, CLKEN_HFOSC, true, &error_code);
    if ((rval != 0) || (error_code != 0)) {
        return;
    }

    /* Enable USB clock */
    rval = SERVICES_clocks_enable_clock(se_services_s_handle, CLKEN_CLK_20M, true, &error_code);
    if ((rval != 0) || (error_code != 0)) {
        return;
    }

    /* Get the current run configuration from SE */
    rval = SERVICES_get_run_cfg(se_services_s_handle, &runp, &error_code);
    if (rval != 0) {
        return;
    }

    /* Enable power to USB phy */
    runp.phy_pwr_gating |= USB_PHY_MASK;

    /* Set the current run configuration to SE */
    rval = SERVICES_set_run_cfg(se_services_s_handle, &runp, &error_code);
    if (rval != 0) {
        return;
    }
}

static void CpuCacheEnable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

int main(void)
{

    /* Apply pin configuration */
    board_pins_config();

    se_services_port_init();

    /* Initialize clocks */
    clock_init();

    /* Initialize STDIO */
    stdio_init();

    /* Initialize Virtual I/O */
    vioInit();

    /* Initialize Ethos NPU */
    NpuInit();

#ifdef CMSIS_shield_header
    shield_setup();
#endif

    /* Enable the CPU Cache */
    CpuCacheEnable();

    return app_main();
}
