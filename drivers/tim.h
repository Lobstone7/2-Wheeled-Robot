#ifndef TIM_H
#define TIM_H

#include "common.h"

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1;
    volatile uint32_t RESERVED2[3];
    volatile uint32_t OR2;
} TIM2_5_TypeDef;

#define TIM2 ((TIM2_5_TypeDef*)(APB1))
#define TIM3 ((TIM2_5_TypeDef*)(APB1 + 0x400))
#define TIM4 ((TIM2_5_TypeDef*)(APB1 + 0x800))
#define TIM5 ((TIM2_5_TypeDef*)(APB1 + 0xC00))

#define TIM_CR1_CEN (1U << 0)
#define TIM_EGR_UG (1U << 0)
#define TIM_SR_UIF (1U << 0)

#define TIM_CR1_ARPE (1U << 7)


void tim2_5_init(TIM2_5_TypeDef* timer,uint32_t PSC, uint32_t ARR);
void tim_start(TIM2_5_TypeDef* timer);
void tim_stop(TIM2_5_TypeDef* timer);
void reset_cnt(TIM2_5_TypeDef* timer);
uint32_t read_cnt(TIM2_5_TypeDef* timer);
void tim_update(TIM2_5_TypeDef* timer);

#endif