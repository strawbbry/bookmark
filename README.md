# bookmark

A bookmark-form PCB that tracks study sessions. 

## Hardware

- MCU: STM32G031K8 (Arm Cortex-M0+)
- I2C: DS3231
- SPI: W25Q32
- OLED: SSD1306
- Voltage Divider: FSR402

## Files

- `firmware/` : STM32CubeIDE project targeting the STM32G031K8, now migrated to baremetal C, developed and tested on a STM32 NUCLEO-G031K8 dev board. See `firmware/README.md`.
- `hardware/` : Altium Designer project (`YPT`) for custom PCB. See `hardware/README.md`.
