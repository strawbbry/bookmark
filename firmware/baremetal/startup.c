
// reset handler
__attribute__((naked, noreturn)) void _reset(void) {  
    // link.ld 
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    // memset .bss to zero
    for (long *dst = &_sbss; dst < &_ebss; dst++) { *dst = 0; }
    // copy .data section to RAM
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata; dst++) { *dst = *src++; }

    extern void main(void); 
    main();

    // infinite loop (if main() returns)
    for (;;) (void) 0;
}

// systick handler 
extern void SysTick_Handler(void); // defined in main.c

// initial stack pointer
extern void _estack(void);         // defined in link.ld

// vector table 'tab' : 16 standard and 32 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};