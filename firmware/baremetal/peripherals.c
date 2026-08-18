#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#define BIT(x) (1UL << (x))  // bitmask
#define FREQ 16000000        // 16Mhz

//                          upper byte       lower byte
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
//                      upper byte
#define PINNUMBER(pin) ((uint8_t)(pin) & 255)
//                   lower byte 
#define PINBANK(pin) ((pin) >> 8)


static inline void spin(volatile uint32_t count) {
    while (count--) (void) 0;  // nop
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


struct rcc {
    volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, RESERVED, CRRCR, CIER, CIFR, CICR, IOPRSTR, AHBRSTR,
        APBRSTR1, APBRSTR2, IOPENR, AHBENR, APBENR1, APBENR2, IOPSMENR, AHBSMENR, APBSMENR1, APBSMENR2, 
        CCIPR, CCIPR2, BDCR, CSR;
};

#define RCC ((struct rcc *) 0x40021000)


struct systick {
    volatile uint32_t CSR, RVR, CVR, CALIB;
};

#define SYSTICK ((struct systick *) 0xE000E010)

static inline void systick_init(uint32_t ticks) {
    if ((ticks - 1) > 0xffffff) { return; }  // 24-bit systick
    
    SYSTICK->RVR = ticks - 1;
    SYSTICK->CVR = 0;
    //             enable  tickint  clksource
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);
    RCC->APBENR2 |= BIT(0);
}


struct gpio {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFRL, AFRH, BRR;
};

#define GPIOA ((struct gpio *) 0x50000000)
#define GPIOB ((struct gpio *) 0x50000400)
#define GPIOC ((struct gpio *) 0x50000800)
#define GPIOD ((struct gpio *) 0x50000C00)
#define GPIOE ((struct gpio *) 0x50001000)
#define GPIOF ((struct gpio *) 0x50001400)

#define GPIO(bank) ((struct gpio *) (0x50000000 + 0x400 * bank))

// num       0                1                        2                     3
enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_ALTERNATEFUNCTION, GPIO_MODE_ANALOG};

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *bank = GPIO(PINBANK(pin));
    uint8_t number = PINNUMBER(pin);

    RCC->IOPENR |= BIT(PINBANK(pin));

    bank->MODER &= ~(3U << (number * 2));
    bank->MODER |= (mode & 3) << (number * 2);
}

static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio *bank = GPIO(PINBANK(pin));
    //                 pin mask      shift to    set reset
    bank->BSRR = (1U << PINNUMBER(pin)) << (val ? 0 : 16);
}

static inline void gpio_set_af(uint16_t pin, uint8_t af) {
    struct gpio *bank = GPIO(PINBANK(pin));
    int number = PINNUMBER(pin);

    //   number / 8
    if ((number >> 3) == 1) {
        //                       & 0111 = 0 - 7 pos
        bank->AFRH &= ~(15UL << ((number & 7) * 4));           // clear pin
        bank->AFRH |= ((uint32_t) af) << ((number & 7) * 4);   // set af
    } else {
        bank->AFRL &= ~(15UL << ((number & 7) * 4));
        bank->AFRL |= ((uint32_t) af) << ((number & 7) * 4);
    }
}


struct uart {
    volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR, PRESC;
};

#define UART1 ((struct uart *) 0x40013800)   // pa9/pa10
#define UART2 ((struct uart *) 0x40004400)   // pa2/pa3

static inline void uart_init(struct uart *uart, unsigned long baud) {
    uint8_t af = 1;
    uint16_t tx = 0;
    uint16_t rx = 0;

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
    uart->BRR = FREQ / baud;
    //             ue       re      te
    uart->CR1 |= BIT(0) | BIT(2) | BIT(3); 
}

static inline int uart_read_ready(struct uart *uart) {
    return uart->ISR & BIT(5);
}

static inline uint8_t uart_read_byte(struct uart *uart) {
    return (uint8_t) (uart->RDR & 255);   // only keep RDR[8:0]
}

static inline void uart_write_byte(struct uart *uart, uint8_t byte) {
    uart->TDR = byte;
    while ((uart->ISR & BIT(7)) == 0) { spin(1); }
}

static inline void uart_write_buffer(struct uart *uart, char *buf, size_t len) {
    while (len-- > 0) { uart_write_byte(uart, *(uint8_t *) buf++); }
}