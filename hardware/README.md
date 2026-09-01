# hardware

Altium Designer project for custom bookmark PCB.

## pcb

Main PCB design.

## nucleo

Recreation of subset of NucleoG031K8 reference schematic (MB1455).

- STM32G031K8T6 (32 pins)
- VDD/VSS decoupling capacitors (100nF, 4.7uF)
- NRST circuit with custom switch symbol and footprint
- SWD header (SWDIO, SWCLK, GND, VDD)

## Status
- `pcb` clean ERC check for W25Q32 Flash module schematic.
- `pcb` clean ERC check for DS3231 RTC module schematic.
- `pcb` clean ERC check for LiPo power tree schematic.
- `pcb` clean ERC check for MCU core schematic.
- `pcb` base project created.
- `nucleo` clean ERC check for NucleoG031K8 recreation schematic.
