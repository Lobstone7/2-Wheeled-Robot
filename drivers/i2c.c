#include "i2c.h"

void i2c1_init(){
    I2C1->CR1 &= ~(1U << 0);

    I2C1->TIMINGR = 0x10909CEC;

    I2C1->CR1 |= (1U << 0);

}

void i2c_write(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t data){
    I2C->CR2  = 0;
    I2C->CR2 |= ((address << 1) & 0x3FFU);

    I2C->CR2 &= ~(1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= (2U << 16);

    I2C->CR2 |= (1U << 25);

    I2C->CR2 |= (1U << 13);

    while(!(I2C->ISR & (1U << 1)));
    I2C->TXDR = reg;

    while(!(I2C->ISR & (1U << 1)));
    I2C->TXDR = data;

    while(!(I2C->ISR & (1U << 5)));
    I2C->ICR = (1U << 4);
    I2C->ICR = (1U << 5);
    I2C->ICR = (1U << 8);
    I2C->ICR = (1U << 9);

}

uint8_t i2c_read(I2C_TypeDef* I2C,uint8_t address,uint8_t reg){
    I2C->CR2  &= ~(0x3FFU);
    I2C->CR2 |= ((address << 1) & 0x3FFU);

    I2C->CR2 &= ~(1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= (1U << 16);

    I2C->CR2 &= ~(1U << 25);

    I2C->CR2 |= (1U << 13);

    while(!(I2C->ISR & (1U << 1)));
    I2C->TXDR = reg;

    while(!(I2C->ISR & (1U << 6)));

    I2C->ICR = (1U << 4);
    I2C->ICR = (1U << 5);

    I2C->CR2 &= ~(0x3FFU);

    I2C->CR2 &= ~(1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= ((address << 1) & 0x3FFU);

    I2C->CR2 |= (1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= (1U << 16);

    I2C->CR2 |= (1U << 25);

    I2C->CR2 |= (1U << 13);

    while(!(I2C->ISR & (1U << 2)));
    uint8_t data = I2C->RXDR;

    while(!(I2C->ISR & (1U << 5)));
    I2C->ICR = (1U << 4);
    I2C->ICR = (1U << 5);
    I2C->ICR = (1U << 8);
    I2C->ICR = (1U << 9);

    return data;
}

void i2c_read_bytes(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t *buffer,uint8_t length){
    
    I2C->CR2  &= ~(0x3FFU);
    I2C->CR2 |= ((address << 1) & 0x3FFU);

    I2C->CR2 &= ~(1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= (1U << 16);

    I2C->CR2 &= ~(1U << 25);

    I2C->CR2 |= (1U << 13);

    while(!(I2C->ISR & (1U << 1)));
    I2C->TXDR = reg;

    while(!(I2C->ISR & (1U << 6)));

    I2C->ICR = (1U << 4);
    I2C->ICR = (1U << 5);

    I2C->CR2 &= ~(0x3FFU);

    I2C->CR2 &= ~(1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= ((address << 1) & 0x3FFU);

    I2C->CR2 |= (1U << 10);

    I2C->CR2 &= ~(0xFFU << 16);
    I2C->CR2 |= (length << 16);

    I2C->CR2 |= (1U << 25);

    I2C->CR2 |= (1U << 13);

    for(uint8_t i =0;i<length;i++){
        uint32_t timeout = 1000000;
        while(!(I2C->ISR & (1U << 2))){
            if (I2C->ISR & (1U << 4)) {
                usart2_write((uint8_t *)"NACK\r\n", 6);
                return;
            }

            if (I2C->ISR & (1U << 8)) {
                usart2_write((uint8_t *)"BERR\r\n", 6);
                return;
            }

            if (I2C->ISR & (1U << 9)) {
                usart2_write((uint8_t *)"ARLO\r\n", 6);
                return;
            }

            if (I2C->ISR & (1U << 10)) {
                usart2_write((uint8_t *)"OVR\r\n", 5);
                return;
            }

            if (--timeout == 0) {
                usart2_write((uint8_t *)"TIMEOUT BYTE:", 13);
                usart2_write_int(i);
                usart2_write((uint8_t *)"\r\n", 2);
                return;
            }
        }
        
        buffer[i] = I2C->RXDR;
    }
    
    while(!(I2C->ISR & (1U << 5)));
    I2C->ICR = (1U << 4);
    I2C->ICR = (1U << 5);
    I2C->ICR = (1U << 8);
    I2C->ICR = (1U << 9);
}