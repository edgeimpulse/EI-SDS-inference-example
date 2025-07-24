# Board: STMicroelectronics [STM32H735G-DK](https://www.st.com/en/evaluation-tools/stm32h735g-dk.html)

## Default Board Layer

Device: **STM32H735IGK6**

System Core Clock: **550 MHz**

This setup is configured using **STM32CubeMX**, an interactive tool provided by STMicroelectronics for device configuration.
Refer to ["Configure STM32 Devices with CubeMX"](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/CubeMX.md) for additional information.

### System Configuration

| System resource       | Setting
|:----------------------|:--------------------------------------
| Heap                  | 64 kB (configured in the STM32CubeMX)
| Stack (MSP)           |  1 kB (configured in the STM32CubeMX)

### STDIO mapping

**STDIO** is routed to Virtual COM port on the ST-LINK (using **USART3** peripheral)

### CMSIS-Driver mapping

| CMSIS-Driver          | Peripheral            | Board connector/component                     | Connection
|:----------------------|:----------------------|:----------------------------------------------|:------------------------------
| Driver_ETH_MAC0       | ETH                   | Ethernet RJ45 connector (CN3)                 | CMSIS_ETH
| Driver_ETH_PHY0       | LAN8742A (external)   | Ethernet RJ45 connector (CN3)                 | CMSIS_ETH
| Driver_GPIO0          | GPIO                  | Arduino digital I/O pins D2..D10, D14..D19    | ARDUINO_UNO_D2..D10, D14..D19
| Driver_I2C4           | I2C4                  | Arduino I2C pins D20..D21                     | ARDUINO_UNO_I2C
| Driver_MCI1           | SDMMC1                | MicroSD connector (CN2)                       | CMSIS_MCI
| Driver_SPI5           | SPI5                  | Arduino SPI pins D10..D13                     | ARDUINO_UNO_SPI
| Driver_USART1         | USART1                | Arduino UART pins D0..D1                      | ARDUINO_UNO_UART
| Driver_USART3         | USART3                | ST-LINK connector (CN15)                      | STDIN, STDOUT, STDERR
| Driver_USBD1          | USB_OTG_HS (FS mode)  | USB OTG FS connector (CN14)                   | CMSIS_USB_Device
| CMSIS-Driver VIO      | GPIO                  | LEDs (LD3, LD1, LD2) and USER button (B1)     | CMSIS_VIO

Reference to [Arduino UNO connector description](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/blob/main/docs/ReferenceApplications.md#arduino-shield).

### CMSIS-Driver Virtual I/O mapping

| CMSIS-Driver VIO      | Board component
|:----------------------|:--------------------------------------
| vioBUTTON0            | USER button (B1)
| vioLED0               | LED red     (LD2)
| vioLED1               | LED green   (LD1)
