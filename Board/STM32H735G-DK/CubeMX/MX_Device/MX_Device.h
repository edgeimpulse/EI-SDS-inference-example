/******************************************************************************
 * File Name   : MX_Device.h
 * Date        : 25/09/2024 13:56:57
 * Description : STM32Cube MX parameter definitions
 * Note        : This file is generated with a generator out of the
 *               STM32CubeMX project and its generated files (DO NOT EDIT!)
 ******************************************************************************/

#ifndef MX_DEVICE_H__
#define MX_DEVICE_H__

/* MX_Device.h version */
#define MX_DEVICE_VERSION                       0x01000000


/*------------------------------ ETH            -----------------------------*/
#define MX_ETH                                  1

/* Pins */

/* ETH_CRS_DV */
#define MX_ETH_CRS_DV_Pin                       PA7
#define MX_ETH_CRS_DV_GPIO_Pin                  GPIO_PIN_7
#define MX_ETH_CRS_DV_GPIOx                     GPIOA
#define MX_ETH_CRS_DV_GPIO_Mode                 GPIO_MODE_AF_PP
#define MX_ETH_CRS_DV_GPIO_PuPd                 GPIO_NOPULL
#define MX_ETH_CRS_DV_GPIO_Speed                GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_CRS_DV_GPIO_AF                   GPIO_AF11_ETH

/* ETH_MDC */
#define MX_ETH_MDC_Pin                          PC1
#define MX_ETH_MDC_GPIO_Pin                     GPIO_PIN_1
#define MX_ETH_MDC_GPIOx                        GPIOC
#define MX_ETH_MDC_GPIO_Mode                    GPIO_MODE_AF_PP
#define MX_ETH_MDC_GPIO_PuPd                    GPIO_NOPULL
#define MX_ETH_MDC_GPIO_Speed                   GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_MDC_GPIO_AF                      GPIO_AF11_ETH

/* ETH_MDIO */
#define MX_ETH_MDIO_Pin                         PA2
#define MX_ETH_MDIO_GPIO_Pin                    GPIO_PIN_2
#define MX_ETH_MDIO_GPIOx                       GPIOA
#define MX_ETH_MDIO_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_ETH_MDIO_GPIO_PuPd                   GPIO_NOPULL
#define MX_ETH_MDIO_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_MDIO_GPIO_AF                     GPIO_AF11_ETH

/* ETH_REF_CLK */
#define MX_ETH_REF_CLK_Pin                      PA1
#define MX_ETH_REF_CLK_GPIO_Pin                 GPIO_PIN_1
#define MX_ETH_REF_CLK_GPIOx                    GPIOA
#define MX_ETH_REF_CLK_GPIO_Mode                GPIO_MODE_AF_PP
#define MX_ETH_REF_CLK_GPIO_PuPd                GPIO_NOPULL
#define MX_ETH_REF_CLK_GPIO_Speed               GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_REF_CLK_GPIO_AF                  GPIO_AF11_ETH

/* ETH_RXD0 */
#define MX_ETH_RXD0_Pin                         PC4
#define MX_ETH_RXD0_GPIO_Pin                    GPIO_PIN_4
#define MX_ETH_RXD0_GPIOx                       GPIOC
#define MX_ETH_RXD0_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_ETH_RXD0_GPIO_PuPd                   GPIO_NOPULL
#define MX_ETH_RXD0_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_RXD0_GPIO_AF                     GPIO_AF11_ETH

/* ETH_RXD1 */
#define MX_ETH_RXD1_Pin                         PC5
#define MX_ETH_RXD1_GPIO_Pin                    GPIO_PIN_5
#define MX_ETH_RXD1_GPIOx                       GPIOC
#define MX_ETH_RXD1_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_ETH_RXD1_GPIO_PuPd                   GPIO_NOPULL
#define MX_ETH_RXD1_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_RXD1_GPIO_AF                     GPIO_AF11_ETH

/* ETH_RX_ER */
#define MX_ETH_RX_ER_Pin                        PB10
#define MX_ETH_RX_ER_GPIO_Pin                   GPIO_PIN_10
#define MX_ETH_RX_ER_GPIOx                      GPIOB
#define MX_ETH_RX_ER_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_ETH_RX_ER_GPIO_PuPd                  GPIO_NOPULL
#define MX_ETH_RX_ER_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_ETH_RX_ER_GPIO_AF                    GPIO_AF11_ETH

/* ETH_TXD0 */
#define MX_ETH_TXD0_Pin                         PB12
#define MX_ETH_TXD0_GPIO_Pin                    GPIO_PIN_12
#define MX_ETH_TXD0_GPIOx                       GPIOB
#define MX_ETH_TXD0_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_ETH_TXD0_GPIO_PuPd                   GPIO_NOPULL
#define MX_ETH_TXD0_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_TXD0_GPIO_AF                     GPIO_AF11_ETH

/* ETH_TXD1 */
#define MX_ETH_TXD1_Pin                         PB13
#define MX_ETH_TXD1_GPIO_Pin                    GPIO_PIN_13
#define MX_ETH_TXD1_GPIOx                       GPIOB
#define MX_ETH_TXD1_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_ETH_TXD1_GPIO_PuPd                   GPIO_NOPULL
#define MX_ETH_TXD1_GPIO_Speed                  GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_TXD1_GPIO_AF                     GPIO_AF11_ETH

/* ETH_TX_EN */
#define MX_ETH_TX_EN_Pin                        PB11
#define MX_ETH_TX_EN_GPIO_Pin                   GPIO_PIN_11
#define MX_ETH_TX_EN_GPIOx                      GPIOB
#define MX_ETH_TX_EN_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_ETH_TX_EN_GPIO_PuPd                  GPIO_NOPULL
#define MX_ETH_TX_EN_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_ETH_TX_EN_GPIO_AF                    GPIO_AF11_ETH

/*------------------------------ FDCAN1         -----------------------------*/
#define MX_FDCAN1                               1

/* Pins */

/* FDCAN1_RX */
#define MX_FDCAN1_RX_Pin                        PH14
#define MX_FDCAN1_RX_GPIO_Pin                   GPIO_PIN_14
#define MX_FDCAN1_RX_GPIOx                      GPIOH
#define MX_FDCAN1_RX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_FDCAN1_RX_GPIO_PuPd                  GPIO_NOPULL
#define MX_FDCAN1_RX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_FDCAN1_RX_GPIO_AF                    GPIO_AF9_FDCAN1

/* FDCAN1_TX */
#define MX_FDCAN1_TX_Pin                        PH13
#define MX_FDCAN1_TX_GPIO_Pin                   GPIO_PIN_13
#define MX_FDCAN1_TX_GPIOx                      GPIOH
#define MX_FDCAN1_TX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_FDCAN1_TX_GPIO_PuPd                  GPIO_NOPULL
#define MX_FDCAN1_TX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_FDCAN1_TX_GPIO_AF                    GPIO_AF9_FDCAN1

/*------------------------------ FDCAN2         -----------------------------*/
#define MX_FDCAN2                               1

/* Pins */

/* FDCAN2_RX */
#define MX_FDCAN2_RX_Pin                        PB5
#define MX_FDCAN2_RX_GPIO_Pin                   GPIO_PIN_5
#define MX_FDCAN2_RX_GPIOx                      GPIOB
#define MX_FDCAN2_RX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_FDCAN2_RX_GPIO_PuPd                  GPIO_NOPULL
#define MX_FDCAN2_RX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_FDCAN2_RX_GPIO_AF                    GPIO_AF9_FDCAN2

/* FDCAN2_TX */
#define MX_FDCAN2_TX_Pin                        PB6
#define MX_FDCAN2_TX_GPIO_Pin                   GPIO_PIN_6
#define MX_FDCAN2_TX_GPIOx                      GPIOB
#define MX_FDCAN2_TX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_FDCAN2_TX_GPIO_PuPd                  GPIO_NOPULL
#define MX_FDCAN2_TX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_FDCAN2_TX_GPIO_AF                    GPIO_AF9_FDCAN2

/*------------------------------ I2C4           -----------------------------*/
#define MX_I2C4                                 1

/* Filter Settings */
#define MX_I2C4_ANF_ENABLE                      1
#define MX_I2C4_DNF                             0

/* Peripheral Clock Frequency */
#define MX_I2C4_PERIPH_CLOCK_FREQ               137500000

/* Pins */

/* I2C4_SCL */
#define MX_I2C4_SCL_Pin                         PF14
#define MX_I2C4_SCL_GPIO_Pin                    GPIO_PIN_14
#define MX_I2C4_SCL_GPIOx                       GPIOF
#define MX_I2C4_SCL_GPIO_Mode                   GPIO_MODE_AF_OD
#define MX_I2C4_SCL_GPIO_PuPd                   GPIO_PULLUP
#define MX_I2C4_SCL_GPIO_Speed                  GPIO_SPEED_FREQ_LOW
#define MX_I2C4_SCL_GPIO_AF                     GPIO_AF4_I2C4

/* I2C4_SDA */
#define MX_I2C4_SDA_Pin                         PF15
#define MX_I2C4_SDA_GPIO_Pin                    GPIO_PIN_15
#define MX_I2C4_SDA_GPIOx                       GPIOF
#define MX_I2C4_SDA_GPIO_Mode                   GPIO_MODE_AF_OD
#define MX_I2C4_SDA_GPIO_PuPd                   GPIO_PULLUP
#define MX_I2C4_SDA_GPIO_Speed                  GPIO_SPEED_FREQ_LOW
#define MX_I2C4_SDA_GPIO_AF                     GPIO_AF4_I2C4

/*------------------------------ SDMMC1         -----------------------------*/
#define MX_SDMMC1                               1

/* Mode */
#define MX_SDMMC1_MODE_SD                       1

/* Peripheral Clock Frequency */
#define MX_SDMMC1_PERIPH_CLOCK_FREQ             137500000

/* Pins */

/* SDMMC1_CK */
#define MX_SDMMC1_CK_Pin                        PC12
#define MX_SDMMC1_CK_GPIO_Pin                   GPIO_PIN_12
#define MX_SDMMC1_CK_GPIOx                      GPIOC
#define MX_SDMMC1_CK_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SDMMC1_CK_GPIO_PuPd                  GPIO_PULLUP
#define MX_SDMMC1_CK_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_CK_GPIO_AF                    GPIO_AF12_SDMMC1

/* SDMMC1_CMD */
#define MX_SDMMC1_CMD_Pin                       PD2
#define MX_SDMMC1_CMD_GPIO_Pin                  GPIO_PIN_2
#define MX_SDMMC1_CMD_GPIOx                     GPIOD
#define MX_SDMMC1_CMD_GPIO_Mode                 GPIO_MODE_AF_PP
#define MX_SDMMC1_CMD_GPIO_PuPd                 GPIO_PULLUP
#define MX_SDMMC1_CMD_GPIO_Speed                GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_CMD_GPIO_AF                   GPIO_AF12_SDMMC1

/* SDMMC1_D0 */
#define MX_SDMMC1_D0_Pin                        PC8
#define MX_SDMMC1_D0_GPIO_Pin                   GPIO_PIN_8
#define MX_SDMMC1_D0_GPIOx                      GPIOC
#define MX_SDMMC1_D0_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SDMMC1_D0_GPIO_PuPd                  GPIO_PULLUP
#define MX_SDMMC1_D0_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_D0_GPIO_AF                    GPIO_AF12_SDMMC1

/* SDMMC1_D1 */
#define MX_SDMMC1_D1_Pin                        PC9
#define MX_SDMMC1_D1_GPIO_Pin                   GPIO_PIN_9
#define MX_SDMMC1_D1_GPIOx                      GPIOC
#define MX_SDMMC1_D1_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SDMMC1_D1_GPIO_PuPd                  GPIO_PULLUP
#define MX_SDMMC1_D1_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_D1_GPIO_AF                    GPIO_AF12_SDMMC1

/* SDMMC1_D2 */
#define MX_SDMMC1_D2_Pin                        PC10
#define MX_SDMMC1_D2_GPIO_Pin                   GPIO_PIN_10
#define MX_SDMMC1_D2_GPIOx                      GPIOC
#define MX_SDMMC1_D2_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SDMMC1_D2_GPIO_PuPd                  GPIO_PULLUP
#define MX_SDMMC1_D2_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_D2_GPIO_AF                    GPIO_AF12_SDMMC1

/* SDMMC1_D3 */
#define MX_SDMMC1_D3_Pin                        PC11
#define MX_SDMMC1_D3_GPIO_Pin                   GPIO_PIN_11
#define MX_SDMMC1_D3_GPIOx                      GPIOC
#define MX_SDMMC1_D3_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SDMMC1_D3_GPIO_PuPd                  GPIO_PULLUP
#define MX_SDMMC1_D3_GPIO_Speed                 GPIO_SPEED_FREQ_VERY_HIGH
#define MX_SDMMC1_D3_GPIO_AF                    GPIO_AF12_SDMMC1

/*------------------------------ SPI5           -----------------------------*/
#define MX_SPI5                                 1

/* Peripheral Clock Frequency */
#define MX_SPI5_PERIPH_CLOCK_FREQ               137500000

/* Pins */

/* SPI5_MISO */
#define MX_SPI5_MISO_Pin                        PF8
#define MX_SPI5_MISO_GPIO_Pin                   GPIO_PIN_8
#define MX_SPI5_MISO_GPIOx                      GPIOF
#define MX_SPI5_MISO_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SPI5_MISO_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI5_MISO_GPIO_Speed                 GPIO_SPEED_FREQ_HIGH
#define MX_SPI5_MISO_GPIO_AF                    GPIO_AF5_SPI5

/* SPI5_MOSI */
#define MX_SPI5_MOSI_Pin                        PF9
#define MX_SPI5_MOSI_GPIO_Pin                   GPIO_PIN_9
#define MX_SPI5_MOSI_GPIOx                      GPIOF
#define MX_SPI5_MOSI_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_SPI5_MOSI_GPIO_PuPd                  GPIO_NOPULL
#define MX_SPI5_MOSI_GPIO_Speed                 GPIO_SPEED_FREQ_HIGH
#define MX_SPI5_MOSI_GPIO_AF                    GPIO_AF5_SPI5

/* SPI5_SCK */
#define MX_SPI5_SCK_Pin                         PF7
#define MX_SPI5_SCK_GPIO_Pin                    GPIO_PIN_7
#define MX_SPI5_SCK_GPIOx                       GPIOF
#define MX_SPI5_SCK_GPIO_Mode                   GPIO_MODE_AF_PP
#define MX_SPI5_SCK_GPIO_PuPd                   GPIO_NOPULL
#define MX_SPI5_SCK_GPIO_Speed                  GPIO_SPEED_FREQ_HIGH
#define MX_SPI5_SCK_GPIO_AF                     GPIO_AF5_SPI5

/*------------------------------ USART1         -----------------------------*/
#define MX_USART1                               1

/* Virtual mode */
#define MX_USART1_VM                            VM_ASYNC
#define MX_USART1_VM_ASYNC                      1

/* Pins */

/* USART1_RX */
#define MX_USART1_RX_Pin                        PB15
#define MX_USART1_RX_GPIO_Pin                   GPIO_PIN_15
#define MX_USART1_RX_GPIOx                      GPIOB
#define MX_USART1_RX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_USART1_RX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART1_RX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_USART1_RX_GPIO_AF                    GPIO_AF4_USART1

/* USART1_TX */
#define MX_USART1_TX_Pin                        PB14
#define MX_USART1_TX_GPIO_Pin                   GPIO_PIN_14
#define MX_USART1_TX_GPIOx                      GPIOB
#define MX_USART1_TX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_USART1_TX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART1_TX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_USART1_TX_GPIO_AF                    GPIO_AF4_USART1

/*------------------------------ USART3         -----------------------------*/
#define MX_USART3                               1

/* Virtual mode */
#define MX_USART3_VM                            VM_ASYNC
#define MX_USART3_VM_ASYNC                      1

/* Pins */

/* USART3_RX */
#define MX_USART3_RX_Pin                        PD9
#define MX_USART3_RX_GPIO_Pin                   GPIO_PIN_9
#define MX_USART3_RX_GPIOx                      GPIOD
#define MX_USART3_RX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_USART3_RX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART3_RX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_USART3_RX_GPIO_AF                    GPIO_AF7_USART3

/* USART3_TX */
#define MX_USART3_TX_Pin                        PD8
#define MX_USART3_TX_GPIO_Pin                   GPIO_PIN_8
#define MX_USART3_TX_GPIOx                      GPIOD
#define MX_USART3_TX_GPIO_Mode                  GPIO_MODE_AF_PP
#define MX_USART3_TX_GPIO_PuPd                  GPIO_NOPULL
#define MX_USART3_TX_GPIO_Speed                 GPIO_SPEED_FREQ_LOW
#define MX_USART3_TX_GPIO_AF                    GPIO_AF7_USART3

/*------------------------------ USB_OTG_HS     -----------------------------*/
#define MX_USB_OTG_HS                           1

/* Handle */
#define MX_USB_OTG_HS_HANDLE                    hpcd_USB_OTG_HS

/* Virtual mode */
#define MX_USB_OTG_HS_VM                        Device_Only_FS
#define MX_USB_OTG_HS_Device_Only_FS            1

/* Pins */

/* USB_OTG_HS_ID */
#define MX_USB_OTG_HS_ID_Pin                    PA10
#define MX_USB_OTG_HS_ID_GPIO_Pin               GPIO_PIN_10
#define MX_USB_OTG_HS_ID_GPIOx                  GPIOA
#define MX_USB_OTG_HS_ID_GPIO_Mode              GPIO_MODE_AF_PP
#define MX_USB_OTG_HS_ID_GPIO_PuPd              GPIO_NOPULL
#define MX_USB_OTG_HS_ID_GPIO_Speed             GPIO_SPEED_FREQ_LOW
#define MX_USB_OTG_HS_ID_GPIO_AF                GPIO_AF10_OTG1_HS

/* USB_OTG_HS_VBUS */
#define MX_USB_OTG_HS_VBUS_Pin                  PA9
#define MX_USB_OTG_HS_VBUS_GPIO_Pin             GPIO_PIN_9
#define MX_USB_OTG_HS_VBUS_GPIOx                GPIOA
#define MX_USB_OTG_HS_VBUS_GPIO_Mode            GPIO_MODE_INPUT
#define MX_USB_OTG_HS_VBUS_GPIO_PuPd            GPIO_NOPULL
#define MX_USB_OTG_HS_VBUS_GPIO_Speed           GPIO_SPEED_FREQ_LOW
#define MX_USB_OTG_HS_VBUS_GPIO_AF              GPIO_AF10_OTG1_HS

#endif  /* MX_DEVICE_H__ */
