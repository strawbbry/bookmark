# ?= default value 
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
			-I. -Iinclude -Icmsis_core/CMSIS/Core/Include -Icmsis_g0/Include \
            -mcpu=cortex-m0plus -mthumb $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c syscalls.c sysinit.c
SOURCES += cmsis_g0/Source/Templates/gcc/startup_stm32g031xx.s

# 'make build' : create firmware.elf & flashable .bin & start arm-none-eabi-gcc compiler with flags
build: firmware.bin

# $@ expands to target : rebuild whenever these files change
firmware.elf: cmsis_core cmsis_g0 hal.h link.ld Makefile $(SOURCES)														
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@                     

# 'make flash' : create firmware.bin & flash to board 
firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

cmsis_core:
	git clone --depth 1 -b v6.3.0 https://github.com/ARM-software/CMSIS_6 $@

cmsis_g0:
	git clone --depth 1 -b v1.4.5 https://github.com/STMicroelectronics/cmsis_device_g0 $@

# remove build artifacts 
clean:
	rm -rf firmware.* cmsis_*