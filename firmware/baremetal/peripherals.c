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
//                          upper byte       lower byte
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
//                      upper byte
#define PINNUMBER(pin) (pin & 255)
//                   lower byte 
#define PINBANK(pin) (pin >> 8)

// num       0                1                        2                     3
enum {GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_ALTERNATEFUNCTION, GPIO_MODE_ANALOG}

static inline void gpio_set_mode(struct gpio *gpio, uint8_t pin, uint8_t mode) {
    gpio->MODER &= ~(3U << (pin * 2));  // clear
    gpio->MODER |= (mode & 3) << (pin * 2);   // set mode
}

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    struct gpio *bank = GPIO(PINBANK(pin));
    uint8_t number = PINNUMBER(pin);

    gpio->MODER &= ~(3U << (number * 2));
    gpio->MODER |= (mode & 3) << (number * 2);
}