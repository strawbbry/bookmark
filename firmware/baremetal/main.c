#include <peripherals.c>
#include <stdint.h>

// systick handler 
static volatile uint32_t systicks;
void SysTick_Handler(void) {
    systicks++;
}

int main(void) {
    uint16_t led = PIN('C', 6);
    RCC->IOPENR |= BIT(PINBANK(led));
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    systick_init(16000000 / 1000);   // tick per 1ms
    uint32_t systicker;
    uint32_t period = 100;   // blink per 100ms

    for (;;) {
        // pc6 led blink
        if (timer(&systicker, period, systicks)) {
            static bool on;
            gpio_write(led, on);
            on = !on;   // toggle led
        }
    };
    return 0; 
}

// reset handler
__attribute__((naked, noreturn)) void _reset(void) {  
    // link.ld 
    extern long _sbss, _ebss, _sdata, _edata, _sidata;
    // memset .bss to zero
    for (long *dst = &_sbss; dst < &_ebss; dst++) { *dst = 0; }
    // copy .data section to RAM
    for (long *dst = &_sdata, *src = &_sidata; dst < &_edata; dst++) { *dst = *src++; }

    main();
    for (;;) (void) 0;  // infinite loop (if main() returns)
}

// initial stack pointer
extern void _estack(void);  // defined in link.ld

// vector table 'tab' : 16 standard and 32 STM32-specific handlers
__attribute__((section(".vectors"))) void (*const tab[16 + 32])(void) = {
    _estack, _reset, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, SysTick_Handler
};