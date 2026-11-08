# makefile hardcodes arm-none-eabi-gcc, 
# but cmake must be told that this is board not PC build

set(CMAKE_SYSTEM_NAME Generic) # (no OS)
set(CMAKE_SYSTEM_PROCESSOR arm) 

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

# test build compiler first ! 
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY) 