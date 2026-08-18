#include <hal.h>

static volatile uint32_t systicks;
void SysTick_Handler(void) {
    systicks++;
}

int main(void) {
    uint16_t led = PIN('C', 6);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);

    systick_init(16000000 / 1000);   // tick per 1ms
    uint32_t systicker = 0;
    uint32_t period = 100;   // blink per 100ms

    uart_init(UART2, 115200);

    for (;;) {
        // pc6 led blink
        if (timer(&systicker, period, systicks)) {
            static bool on;
            gpio_write(led, on);
            on = !on;   // toggle led

            // i/o retarget _write() to uart_write_buffer
            printf("blink!! led: %d, tick: %lu\r\n", on, systicks);   
        }
    };
    return 0; 
}