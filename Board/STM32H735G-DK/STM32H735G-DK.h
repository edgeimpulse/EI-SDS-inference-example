/*---------------------------------------------------------------------------
 * Copyright (c) 2024 Arm Limited (or its affiliates).
 * All rights reserved.
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

#ifndef STM32H735G_DK_H_
#define STM32H735G_DK_H_

#include "GPIO_STM32.h"
#include "Driver_ETH_MAC.h"
#include "Driver_ETH_PHY.h"
#include "Driver_I2C.h"
#include "Driver_MCI.h"
#include "Driver_SPI.h"
#include "Driver_USART.h"
#include "Driver_USBD.h"

// Arduino Connector Digital I/O
#define ARDUINO_UNO_D2      GPIO_PIN_ID_PORTG(3U)
#define ARDUINO_UNO_D3      GPIO_PIN_ID_PORTA(0U)
#define ARDUINO_UNO_D4      GPIO_PIN_ID_PORTG(4U)
#define ARDUINO_UNO_D5      GPIO_PIN_ID_PORTE(14U)
#define ARDUINO_UNO_D6      GPIO_PIN_ID_PORTD(15U)
#define ARDUINO_UNO_D7      GPIO_PIN_ID_PORTG(5U)
#define ARDUINO_UNO_D8      GPIO_PIN_ID_PORTE(3U)
#define ARDUINO_UNO_D9      GPIO_PIN_ID_PORTB(7U)
#define ARDUINO_UNO_D10     GPIO_PIN_ID_PORTF(6U)
#define ARDUINO_UNO_D14     GPIO_PIN_ID_PORTC(0U)
#define ARDUINO_UNO_D15     GPIO_PIN_ID_PORTH(2U)
#define ARDUINO_UNO_D16     GPIO_PIN_ID_PORTA(0U)
#define ARDUINO_UNO_D17     GPIO_PIN_ID_PORTA(1U)
#define ARDUINO_UNO_D18     GPIO_PIN_ID_PORTC(2U)
#define ARDUINO_UNO_D19     GPIO_PIN_ID_PORTC(3U)

// CMSIS Driver instances on Arduino Connector
#define ARDUINO_UNO_I2C     4           // I2C4
#define ARDUINO_UNO_SPI     5           // SPI5
#define ARDUINO_UNO_UART    1           // USART1

// CMSIS Driver instances of Board peripherals
#define CMSIS_DRIVER_ETH    0           // Driver_ETH_MAC0
#define CMSIS_DRIVER_MCI    1           // Driver_MCI1
#define CMSIS_DRIVER_USBD   1           // Driver_USBD1

// CMSIS Driver instance for STDIO retarget
#define RETARGET_STDIO_UART 3

// CMSIS Drivers
extern ARM_DRIVER_ETH_MAC   ARM_Driver_ETH_MAC_(CMSIS_DRIVER_ETH);      // ETH MAC
extern ARM_DRIVER_ETH_PHY   ARM_Driver_ETH_PHY_(CMSIS_DRIVER_ETH);      // ETH PHY
extern ARM_DRIVER_I2C       ARM_Driver_I2C_(ARDUINO_UNO_I2C);           // Arduino I2C
extern ARM_DRIVER_MCI       ARM_Driver_MCI_(CMSIS_DRIVER_MCI);          // MCI
extern ARM_DRIVER_SPI       ARM_Driver_SPI_(ARDUINO_UNO_SPI);           // Arduino SPI
extern ARM_DRIVER_USART     ARM_Driver_USART_(ARDUINO_UNO_UART);        // Arduino UART
extern ARM_DRIVER_USART     ARM_Driver_USART_(RETARGET_STDIO_UART);     // ST-LINK
extern ARM_DRIVER_USBD      ARM_Driver_USBD_(CMSIS_DRIVER_USBD);        // USB Device

#ifdef   CMSIS_shield_header
#include CMSIS_shield_header
#endif

#endif // STM32H735G_DK_H_
