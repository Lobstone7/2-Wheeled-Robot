#ifndef GPIO_H
#define GPIO_H
#include "common.h"

#define RCC_AHB2ENR_OFFSET (0x4C)
#define RCC_AHB2ENR (*(volatile uint32_t*)(RCC + RCC_AHB2ENR_OFFSET))

#define GPIOA_BASE (AHB2)
#define GPIOB_BASE (AHB2 + 0x400U)
#define GPIOC_BASE (AHB2 + 0x800U)

typedef struct{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
    volatile uint32_t BRR;
    volatile uint32_t ASCR;
}GPIO_Typedef;

typedef enum {
    INPUT_MODE,
    OUTPUT_MODE,
    ALT_FUNC_MODE,
    ANALOG_MODE
} GPIO_Mode;

typedef enum {
    GPIO_AF0  = 0b0000,
    GPIO_AF1  = 0b0001,
    GPIO_AF2  = 0b0010,
    GPIO_AF3  = 0b0011,
    GPIO_AF4  = 0b0100,
    GPIO_AF5  = 0b0101,
    GPIO_AF6  = 0b0110,
    GPIO_AF7  = 0b0111,
    GPIO_AF8  = 0b1000,
    GPIO_AF9  = 0b1001,
    GPIO_AF10 = 0b1010,
    GPIO_AF11 = 0b1011,
    GPIO_AF12 = 0b1100,
    GPIO_AF13 = 0b1101,
    GPIO_AF14 = 0b1110,
    GPIO_AF15 = 0b1111
} GPIO_AlternateFunction;

typedef struct{
    GPIO_Typedef* port;
    volatile uint32_t pin;
}GPIO_pin;


#define GPIOA ((GPIO_Typedef*)GPIOA_BASE)
#define GPIOB ((GPIO_Typedef*)GPIOB_BASE)
#define GPIOC ((GPIO_Typedef*)GPIOC_BASE)

#define GPIO_PORT_SPACE 0x400U

void rcc_gpioa_enable();
void rcc_gpiob_enable();
void rcc_gpioc_enable();
void gpio_pupd(GPIO_Typedef *port, uint32_t pin, uint32_t mode);
void gpio_init(GPIO_Typedef *port, uint32_t pin, uint32_t mode);
void gpio_alt(GPIO_Typedef *port, uint32_t pin, GPIO_AlternateFunction AFR);
void gpio_write(GPIO_Typedef *port, uint32_t pin, uint32_t value);
uint32_t gpio_read(GPIO_Typedef *port, uint32_t pin);


#endif



