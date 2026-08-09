ENTRY(_reset);                                                            /* entry name per vector table (for debugger) */
MEMORY {                                                                  /* memory region : address, size */
  flash(rx)  : ORIGIN = 0x08000000, LENGTH = 64k
  sram(rwx) : ORIGIN = 0x20000000, LENGTH = 8k                         
}
_estack     = ORIGIN(sram) + LENGTH(sram);                                /* initial stack pointer points to end of SRAM */

SECTIONS {
  .vectors  : { KEEP(*(.vectors)) }   > flash                             /* 1. vector table to flash */
  .text     : { *(.text*) }           > flash                             /* 2. .text to flash */
  .rodata   : { *(.rodata*) }         > flash                             /* 3. .(readonly)data section to flash (LMA) */

  .data : {
    _sdata = .;                                                           /* .(start)data section */
    *(.first_data)
    *(.data SORT(.data.*))
    _edata = .;                                                           /* .(end)data section */
  } > sram AT > flash                                                     /* place .data in SRAM (VMA) for execution, but physically store initializer bytes at flash (LMA) */
  _sidata = LOADADDR(.data);                                              /* (source of initialised)data load from flash */

  .bss : {
    _sbss = .;                                                            /* .(start)bss section */
    *(.bss SORT(.bss.*) COMMON)
    _ebss = .;                                                            /* .(end)bss section */
  } > sram

  . = ALIGN(8);
  _end = .;                                                               /* for cmsis_gcc.h  */
}