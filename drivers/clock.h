#ifndef CLOCK_H
#define CLOCK_H

#include "common.h"

#define RCC_OFFSET (0x00001000U)
#define RCC_BASE (AHB1 + RCC_OFFSET)

typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t PLLSAI2CFGR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    uint32_t RESERVED0;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED1;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED2;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED3;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
    uint32_t RESERVED4;
    volatile uint32_t AHB1SMENR;
    volatile uint32_t AHB2SMENR;
    volatile uint32_t AHB3SMENR;
    uint32_t RESERVED5;
    volatile uint32_t APB1SMENR1;
    volatile uint32_t APB1SMENR2;
    volatile uint32_t APB2SMENR;
    uint32_t RESERVED6;
    volatile uint32_t CCIPR;
    uint32_t RESERVED7;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t CRRCR;
    volatile uint32_t CCIPR2;
} RCC_TypeDef;

#define RCC ((RCC_TypeDef*)RCC_BASE)

void rcc_gpioa_enable();
void rcc_gpiob_enable();
void rcc_gpioc_enable();

void rcc_usart2_enable();

void rcc_tim2_enable();
void rcc_tim3_enable();
void rcc_tim4_enable();
void rcc_tim5_enable();

void rcc_syscfg_enable();

void rcc_i2c_enable();

void SystemClock_Config(void);

#endif