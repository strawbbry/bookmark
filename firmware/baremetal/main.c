// reset handler
__attribute__((naked, noreturn)) void _reset(void) {  
    for (;;) (void) 0;  // infinite loop
}

// initial stack pointer
extern void _estack(void);  // defined in link.ld

// vector table 'tab' : 16 standard and 32 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    _estack, _reset
};