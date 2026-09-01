# firmware

Firmware for the STM32G031K8, built and run on a NUCLEO-G031K8 dev board (LQFP32 package).

## Files

- `baremetal` : baremetal C base code created by learning with [baremetal guide](https://github.com/cpq/bare-metal-programming-guide/tree/main).
- `stm32cube` : HAL-based C code to validate peripherals built upon CubeMX-generated configuration in STM32CubeIDE.
- `demo.mov` : Peripheral validation via `stm32cube` code demonstration video.

## Guide

Connect to Nucleo:
```bash
ls /dev/cu.*   # 
cu -l /dev/cu.<devboard> -s 115200
# ~. to exit
```
Run baremetal code:
```bash
cd ~/firmware/baremetal
make clean && make build
make flash
```
To run HAL-based peripheral validation code, flash Nucleo using STM32CubeIDE.

## Configuration

Peripherals configured via CubeMX (consistent in baremetal):
- GPIO (LD3): on-board LED (PC6)
- USART2: UART 8N1 115200 (PA2/PA3)
- I2C1: SCL (PA9), SDA (PA10)
- SPI1: CS (PB9), SCK (PB3), MOSI (PB5), MISO (PB4)

## Status
- `stm32cube` SSD1306 OLED module validation via I2C completed.
- `stm32cube` W25Q32 Flash module validation via SPI in progress.
- `stm32cube` DS3231 RTC module validation via I2C completed.
- `stm32cube` I2C, SPI, clock setup completed.
- `baremetal` maximum frequency clock setup completed.
- `baremetal` CMSIS header migration completed.
- `baremetal` printf() I/O retargeted to UART completed.
- `baremetal` UART string RX test completed.
- `baremetal` SysTick LED test completed.
- `baremetal` LED test completed.
- `stm32cube` LED test completed.
- `stm32cube` UART string TX/RX test completed.
