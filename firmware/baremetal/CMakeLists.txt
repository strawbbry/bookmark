cmake_minimum_required(VERSION 3.20)
project(firmware C ASM)

# SOURCES first because target 'firmware' must exist before other commands 

# SOURCES
set(SOURCES main.c syscalls.c sysinit.c
    cmsis_g0/Source/Templates/gcc/startup_stm32g031xx.s)
add_executable(firmware ${SOURCES})

# firmware.elf
set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
    LINK_DEPENDS ${CMAKE_SOURCE_DIR}/link.ld)

# CFLAGS
target_compile_options(firmware PRIVATE
    -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion
    -Wformat-truncation -fno-common -Wconversion
    -g3 -Os -ffunction-sections -fdata-sections
    -mcpu=cortex-m0plus -mthumb)

# CFLAGS -I
target_include_directories(firmware PRIVATE
    . include cmsis_core/CMSIS/Core/Include cmsis_g0/Include)

# LDFLAGS
target_link_options(firmware PRIVATE
    -mcpu=cortex-m0plus -mthumb
    -T${CMAKE_SOURCE_DIR}/link.ld # path in linker flag is relative to build dir 
    -nostartfiles -nostdlib --specs=nano.specs
    -Wl,--gc-sections
    -Wl,-Map=$<TARGET_FILE:firmware>.map)

# -lc -lgcc
target_link_libraries(firmware PRIVATE c gcc)

# firmware.bin
add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:firmware>
            $<TARGET_FILE_DIR:firmware>/firmware.bin)

# flash
add_custom_target(flash
    COMMAND st-flash --reset write $<TARGET_FILE_DIR:firmware>/firmware.bin 0x8000000
    DEPENDS firmware)