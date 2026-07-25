#ifndef EXTI_H
#define EXTI_H

#include "common.h"
#include "gpio.h"

#define EXTI_BASE (APB2 + 0x400)

typedef struct
{
    volatile uint32_t IMR1;
    volatile uint32_t EMR1;
    volatile uint32_t RTSR1;
    volatile uint32_t FTSR1;
    volatile uint32_t SWIER1;
    volatile uint32_t PR1;
    volatile uint32_t IMR2;
    volatile uint32_t EMR2;
    volatile uint32_t RTSR2;
    volatile uint32_t FTSR2;
    volatile uint32_t SWIER2;
    volatile uint32_t PR2;
} EXTI_TypeDef;

typedef struct
{
    volatile uint32_t MEMRMP;
    volatile uint32_t CFGR1;
    volatile uint32_t EXTICR1;
    volatile uint32_t EXTICR2;
    volatile uint32_t EXTICR3;
    volatile uint32_t EXTICR4;
    volatile uint32_t SCSR;
    volatile uint32_t CFGR2;
    volatile uint32_t SWPR;
    volatile uint32_t SKR;
} SYSCFG_TypeDef;

typedef enum{
    RISING,
    FALLING,
    BOTH,
}Edge_Type;

typedef enum
{
    EXTI0_IRQn      = 6,
    EXTI1_IRQn      = 7,
    EXTI2_IRQn      = 8,
    EXTI3_IRQn      = 9,
    EXTI4_IRQn      = 10,
    EXTI9_5_IRQn    = 23,
    EXTI15_10_IRQn  = 40,
}EXTI_IRQn_t;

typedef struct
{
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    uint32_t RESERVED4[56];
    volatile uint8_t IP[240];
    uint32_t RESERVED5[644];
    volatile uint32_t STIR;
} NVIC_TypeDef;

typedef void (*exticallback)(void *ctx);

#define NVIC_BASE ((uint32_t)0xE000E100UL)
#define NVIC ((NVIC_TypeDef *)NVIC_BASE)

#define SYSCFG_OFFSET (0x00)
#define SYSCFG_BASE (SYSCFG_OFFSET + APB2)
#define SYSCFG ((SYSCFG_TypeDef*)(SYSCFG_BASE))

#define EXTI ((EXTI_TypeDef*)EXTI_BASE)

void exti_register_callback(exticallback cb,uint32_t line,void *ctx);
void rcc_exti_enable();
void exti_init(GPIO_pin *pin,Edge_Type trigger);
void EXTI0_Handler(void);
void EXTI1_Handler(void);
void EXTI2_Handler(void);
void EXTI3_Handler(void);
void EXTI4_Handler(void);
void EXTI10_5_Handler(void);
void EXTI15_9_Handler(void);

#endif