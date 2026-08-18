#include "hal.h"

// CMSIS declares SystemCoreClock & SystemInit as extern: define once ! 

uint32_t SystemCoreClock = SYS_FREQUENCY;

void SystemInit(void) {
    FLASH->ACR |= FLASH_LATENCY | BIT(8) | BIT(9);
    
    RCC->PLLCFGR = 0;

    // PLLSRC = 10 : HSI16
    RCC->PLLCFGR |= (2UL << 0);
    
    // PLLM = 000 : /1

    // PLLN = 0001000 : /8
    RCC->PLLCFGR |= (8UL << 8);

    // PLLR = 001 : /2
    RCC->PLLCFGR |= (1UL << 29);

    RCC->PLLCFGR |= BIT(28);

    RCC->CR |= BIT(24);
    while ((RCC->CR & BIT(25)) == 0) {
        spin(1);
    }

    // PPRE/HPRE = 0xx : /1
    RCC->CFGR = 0;

    // SW = 010 : PLL
    RCC->CFGR |= 2;
    // SWS = 010 : PLL
    while ((RCC->CFGR & (7UL << 3)) != (2UL << 3)) {
        spin(1);
    }

    RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN;
    SysTick_Config(SystemCoreClock / 1000); 
}