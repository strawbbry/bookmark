# firmware

Project for the STM32G031K8, built and run on a NUCLEO-G031K8 dev board (LQFP32 package).

## Files

- `baremetal` : baremetal C code created by learning with [baremetal guide](https://github.com/cpq/bare-metal-programming-guide/tree/main).
- `stm32cube` : HAL-based C code built upon CubeMX-generated configuration in STM32CubeIDE.

## Guide

```bash
cd ~/firmware/baremetal
make clean && make build
make flash

ls /dev/cu.*   # to discover nucleo devboard
cu -l /dev/cu.<devboard> -s 115200
# ~. to exit
```

## Configuration

Peripherals configured via CubeMX (continued via baremetal):
- GPIO (LD3): on-board LED (PC6)
- USART2: UART 8N1 115200 (PA2/PA3)

## Status
- `baremetal` maximum frequency clock setup completed.
- `baremetal` CMSIS header migration completed.
- `baremetal` printf() I/O retargeted to UART completed.
- `baremetal` UART string RX test completed.
- `baremetal` SysTick LED test completed.
- `baremetal` LED test completed.
- `stm32cube` LED test completed.
- `stm32cube` UART string TX/RX test completed.
