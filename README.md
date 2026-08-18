# bookmark

A bookmark-form PCB that tracks study sessions. 

## Hardware

- MCU: STM32G031K8 (Arm Cortex-M0+)

## Files

- `firmware/` : STM32CubeIDE project targeting the STM32G031K8, now migrated to baremetal C, developed and tested on a STM32 NUCLEO-G031K8 dev board. See `firmware/README.md`.
- `hardware/` : Altium Designer project (`YPT`) for custom PCB. See `hardware/README.md`.
