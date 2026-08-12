#include <inttypes.h>
#include <stdbool.h>

#define BIT(x) (1UL << (x))  // bitmask


//                          upper byte       lower byte
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
//                      upper byte
#define PINNUMBER(pin) ((uint8_t)(pin) & 255)
//                   lower byte 
#define PINBANK(pin) ((pin) >> 8)


static inline void spin(volatile uint32_t count) {
    while (count--) (void) 0;  // nop
}

bool timer(uint32_t *expiry, uint32_t period, uint32_t now) {
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

#define SYSTICK ((struct systick *) 0x0000003C)

static inline void systick_init(uint32_t ticks) {
    if ((ticks - 1) > 0xffffff) { return; }  // 24-bit systick
    
    SYSTICK->RVR = ticks - 1;
    SYSTICK->CVR = 0;
    //             enable  tickint  clksource
    SYSTICK->CSR = BIT(0) | BIT(1) | BIT(2);
    RCC->APBRSTR2 |= BIT(0);
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

    bank->MODER &= ~(3U << (number * 2));
    bank->MODER |= (mode & 3) << (number * 2);
}

static inline void gpio_write(uint16_t pin, bool val) {
    struct gpio *bank = GPIO(PINBANK(pin));
    //                 pin mask      shift to    set reset
    bank->BSRR = (1U << PINNUMBER(pin)) << (val ? 0 : 16);
}