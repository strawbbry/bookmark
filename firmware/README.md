# firmware

STM32CubeIDE project for the STM32G031K8, built and run on a NUCLEO-G031K8 dev board (LQFP32 package).

## Files

- `STM32G031K8/Core/` : application code 
- `STM32G031K8/Drivers/` : CMSIS + STM32G0xx HAL
- `STM32G031K8/STM32G031K8.ioc` : CubeMX configuration

## Configuration

Peripherals configured via CubeMX:
- GPIO (LD3): on-board LED (PC6)
- USART2: UART 8N1 115200 (PA2/PA3)
- NRST (T_NRST): hardware reset (ST-LINK)

## Status
- HAL-based LED test completed.
- HAL-based UART string TX/RX test completed.
