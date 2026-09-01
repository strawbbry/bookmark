# bookmark

A bookmark-form PCB that tracks study sessions. 

## Hardware

- MCU: STM32G031K8 (Arm Cortex-M0+)
- RTC module: DS3231
- Flash module: W25Q32
- OLED: SSD1306
- Force Sensor: FSR402

## Files

- `firmware/` : STM32CubeIDE peripheral validation project targeting the STM32G031K8, with separate baremetal C base code, developed and tested on a STM32 NUCLEO-G031K8 dev board. See `firmware/README.md`.
- `hardware/` : Altium Designer project with schematic capture and layout design for custom PCB. See `hardware/README.md`.
- `pinout.png` : MCU pin configuration in STM32CubeMX. Consistent across `firmware` and `hardware`.
