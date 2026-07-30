#ifndef SYSTICK_H
#define SYSTICK_H

#include "common.h"

typedef struct
{
    volatile uint32_t CSR;
    volatile uint32_t RVR;
    volatile uint32_t CVR;
    volatile const uint32_t CALIB;
} SysTick_TypeDef;

#define SYSTICK_BASE    (0xE000E010UL)
#define SysTick         ((SysTick_TypeDef *)SYSTICK_BASE)

void SysTick_init();
void SysTick_Handler(void);
uint32_t SysTick_get_ms();

#endif