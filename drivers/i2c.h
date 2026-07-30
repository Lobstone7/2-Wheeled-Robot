#ifndef I2C_H
#define I2C_H

#include "common.h"
#include "gpio.h"
#include "usart.h"

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
} I2C_TypeDef;

#define I2C1_OFFSET (0x5400U)
#define I2C1_BASE (APB1 + I2C1_OFFSET)
#define I2C1 ((I2C_TypeDef*)I2C1_BASE)

#define I2C2_OFFSET (0x5800U)
#define I2C2_BASE (APB1 + I2C2_OFFSET)
#define I2C2 ((I2C_TypeDef*)I2C2_BASE)

#define I2C3_OFFSET (0x5C00U)
#define I2C3_BASE (APB1 + I2C3_OFFSET)
#define I2C3 ((I2C_TypeDef*)I2C3_BASE)

void i2c1_init();
void i2c_write(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t data);
uint8_t i2c_read(I2C_TypeDef* I2C,uint8_t address,uint8_t reg);
void i2c_read_bytes(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t *buffer,uint8_t length);

#endif