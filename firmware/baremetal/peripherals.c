#include <inttypes.h>
#include <stdbool.h>

#define BIT(x) (1UL << (x))
//                          upper byte       lower byte
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
//                      upper byte
#define PINNUMBER(pin) ((uint8_t)(pin) & 255)
//                   lower byte 
#define PINBANK(pin) ((pin) >> 8)

static inline void spin(volatile uint32_t count) {
    while (count--) (void) 0;  // nop
}


struct rcc {
    volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, RESERVED, CRRCR, CIER, CIFR, CICR, IOPRSTR, AHBRSTR,
        APBRSTR1, APBRSTR2, IOPENR, AHBENR, APBENR1, APBENR2, IOPSMENR, AHBSMENR, APBSMENR1, APBSMENR2, 
        CCIPR, CCIPR2, BDCR, CSR;
};

#define RCC ((struct rcc *) 0x40021000)


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

    bank->MODER &= ~(3U << (number * 2));
    bank->MODER |= (mode & 3) << (number * 2);
}

static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio *bank = GPIO(PINBANK(pin));
    //                 pin mask      shift to    set reset
    bank->BSRR = (1U << PINNUMBER(pin)) << (val ? 0 : 16);
}