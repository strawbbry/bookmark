// only include file once per compilation unit ! 
#pragma once   // = #ifndef HAL #define HAL ... #endif

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

// vendor cmsis headers
#include "stm32g031xx.h"

#define BIT(x) (1UL << (x))  // bitmask
#define FREQ 16000000        // 16Mhz

//                          upper byte       lower byte
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
//                      upper byte
#define PINNUMBER(pin) ((uint8_t)(pin) & 255)
//                   lower byte 
#define PINBANK(pin) ((pin) >> 8)


enum { APB_PRE = 0 };                                     // AHB clock / 1 
enum { PLL_HSI = 16, PLL_M = 1, PLL_N = 8, PLL_R = 2 };   // run @ 64Mhz
#define FLASH_LATENCY 2
#define SYS_FREQUENCY ((PLL_HSI * PLL_N / PLL_M / PLL_R) * 1000000)
#define APB_FREQUENCY (SYS_FREQUENCY)


static inline void spin(volatile uint32_t count) {
    while (count--) { asm("nop"); }   // call assembly fn
}

static inline bool timer(uint32_t *expiry, uint32_t period, uint32_t now) {
    // reset error timer  
    if (now + period < *expiry) { *expiry = 0; }
    // set new timer 
    if (*expiry == 0) { *expiry = now + period; }
    // continue unexpired timer 
    if (*expiry > now) { return false; }
    // expired timer                         set new       resync overdue
    *expiry = ((now - *expiry) > period) ? now + period : *expiry + period;   
    return true;
}


#define GPIO(bank) ((GPIO_TypeDef *) (GPIOA_BASE + 0x400U * (bank)))

enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_ALTERNATEFUNCTION, GPIO_MODE_ANALOG};

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    GPIO_TypeDef *bank = GPIO(PINBANK(pin));
    uint8_t number = PINNUMBER(pin);

    RCC->IOPENR |= BIT(PINBANK(pin));

    bank->MODER &= ~(3U << (number * 2));
    bank->MODER |= (mode & 3) << (number * 2);
}

static inline void gpio_write(uint16_t pin, bool val) {
     GPIO_TypeDef *bank = GPIO(PINBANK(pin));
    //                 pin mask      shift to    set reset
    bank->BSRR = (1U << PINNUMBER(pin)) << (val ? 0 : 16);
}

// GPIO_TypeDef stores AF[2] instead of AFRL, AFRH
static inline void gpio_set_af(uint16_t pin, uint8_t af) {
     GPIO_TypeDef *bank = GPIO(PINBANK(pin));
    int number = PINNUMBER(pin);

    //   number / 8
    if ((number >> 3) == 1) {
        //                       & 0111 = 0 - 7 pos
        bank->AFR[1] &= ~(15UL << ((number & 7) * 4));           // clear pin
        bank->AFR[1] |= ((uint32_t) af) << ((number & 7) * 4);   // set af
    } else {
        bank->AFR[0] &= ~(15UL << ((number & 7) * 4));
        bank->AFR[0] |= ((uint32_t) af) << ((number & 7) * 4);
    }
}


#define UART1 USART1
#define UART2 USART2

static inline void uart_init(USART_TypeDef *uart, unsigned long baud) {
    uint8_t af = 1;
    uint16_t tx = 0;
    uint16_t rx = 0;

    uint32_t freq = SYS_FREQUENCY;

    if (uart == UART1) {
        RCC->APBENR2 |= BIT(14);
        tx = PIN('A', 9);
        rx = PIN('A', 10);
    }
    if (uart == UART2) {
        RCC->APBENR1 |= BIT(17);
        tx = PIN('A', 2);
        rx = PIN('A', 3);
    }

    RCC->IOPENR |= BIT(PINBANK(tx));
    RCC->IOPENR |= BIT(PINBANK(rx));

    gpio_set_mode(tx, GPIO_MODE_ALTERNATEFUNCTION);
    gpio_set_af(tx, af);

    gpio_set_mode(rx, GPIO_MODE_ALTERNATEFUNCTION);
    gpio_set_af(rx, af);

    uart->CR1 = 0;
    //        divisor register 
    uart->BRR = freq / baud;
    //             ue       re      te
    uart->CR1 |= BIT(0) | BIT(2) | BIT(3); 
}

static inline int uart_read_ready(USART_TypeDef *uart) {
    return uart->ISR & BIT(5);
}

static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
    return (uint8_t) (uart->RDR & 255);   // only keep RDR[8:0]
}

static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & BIT(7)) == 0) { spin(1); }
}

static inline void uart_write_buffer(USART_TypeDef *uart, char *buf, size_t len) {
    while (len-- > 0) { uart_write_byte(uart, *(uint8_t *) buf++); }
}