#include "clock.h"

void rcc_gpioa_enable(){
    RCC->AHB2ENR |= (1U << 0);
}

void rcc_gpiob_enable(){
    RCC->AHB2ENR |= (1U << 1);
}

void rcc_gpioc_enable(){
    RCC->AHB2ENR |= (1U << 2);
}

void rcc_usart2_enable(){
    RCC->APB1ENR1 |= (1U << 17);
}

void rcc_tim2_enable(){
    RCC->APB1ENR1 |= (1U << 0);
}

void rcc_tim3_enable(){
    RCC->APB1ENR1 |= (1U << 1);
}

void rcc_tim4_enable(){
    RCC->APB1ENR1 |= (1U << 2);
}

void rcc_tim5_enable(){
    RCC->APB1ENR1 |= (1U << 3);
}

void rcc_syscfg_enable(){
    RCC->APB2ENR |= (1U << 0);
}

void rcc_i2c_enable(){
    RCC->APB1ENR1 |= (1U << 21);
}


void SystemClock_Config(void){
    // Set flash latency to 4 wait states for 80MHz
    FLASH_ACR |= (4U << 0);

    // Set MSI range to 4MHz (range 6)
    RCC->CR |= (1U << 3);        // MSIPLLEN
    RCC->CR = (RCC->CR & ~(0xFU << 4)) | (6U << 4);  // MSIRANGE = 6

    // Configure PLL: M=1, N=40, R=2, source=MSI
    RCC->PLLCFGR = (1U << 0)    // MSI as PLL source
                | (0U << 4)    // PLLM = 1 (value 0 = divide by 1)
                | (40U << 8)   // PLLN = 40
                | (0U << 25)   // PLLR = 2 (value 0 = divide by 2)
                | (1U << 24);  // PLLREN — enable PLLR output

    // Enable PLL
    RCC->CR |= (1U << 24);
    // Wait for PLL to lock
    while(!(RCC->CR & (1U << 25)));

    // Switch SYSCLK to PLL
    RCC->CFGR = (RCC->CFGR & ~(0x3U << 0)) | (3U << 0);
    // Wait until PLL is used
    while((RCC->CFGR & (0x3U << 2)) != (3U << 2));
}