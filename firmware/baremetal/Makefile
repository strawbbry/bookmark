# ?= default value 
CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m0plus -mthumb $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c 

# 'make build' : create firmware.elf & start arm-none-eabi-gcc compiler with flags
build: firmware.elf

# $@ expands to target (firmware.elf)
firmware.elf: $(SOURCES)														
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@                     

# 'make flash' : create firmware.bin & flash to board 
firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --reset write $< 0x8000000

# remove build artifacts 
clean:
	rm -rf firmware.*