# hardware

Altium Designer project for custom bookmark PCB.

## Files

- `pcb` : main PCB design.
- `nucleo` : recreation of subset of NucleoG031K8 reference schematic (MB1455).

## pcb 
- STM32G031K8T6 MCU
- MCP73831 Linear Charge Controller
- AMS1117 Low Dropout Regulator
- CX90M USB-C Charger
- DS3231 RTC via I2C
- W25Q32 Flash via SPI
- SSD1306 OLED via I2C
- FSR402 Force Sensor

Battery & USB-C circuit created by following this [guide](https://www.ampheo.com/blog/how-to-build-a-battery-charging-circuit-with-charger-ics)

## `nucleo`

- STM32G031K8T6 (32 pins)
- VDD/VSS decoupling capacitors (100nF, 4.7uF)
- NRST circuit with custom switch symbol and footprint
- SWD header (SWDIO, SWCLK, GND, VDD)

## Status
- `pcb` base layout footprints (ratsnest) created.
- `pcb` clean ERC check for complete schematic.
- `pcb` clean ERC check for USB-C charging receptacle schematic.
- `pcb` clean ERC check for FSR402 Force sensor schematic.
- `pcb` clean ERC check for SSD1306 OLED module schematic.
- `pcb` clean ERC check for W25Q32 Flash module schematic.
- `pcb` clean ERC check for DS3231 RTC module schematic.
- `pcb` clean ERC check for LiPo power tree schematic.
- `pcb` clean ERC check for MCU core schematic.
- `pcb` base project created.
- `nucleo` clean ERC check for NucleoG031K8 recreation schematic.
