#include "gpio.h"

void gpio_pupd(GPIO_TypeDef *port, uint32_t pin, uint32_t mode){
    port->PUPDR |= (mode << (pin * 2));
}

void gpio_init(GPIO_TypeDef *port, uint32_t pin, uint32_t mode){
    port->MODER &= ~(3U << (pin * 2));
    port->MODER |= (mode << (pin * 2));
}

void gpio_alt(GPIO_TypeDef *port, uint32_t pin, GPIO_AlternateFunction AFR){
    
    if(pin<8){
        uint32_t shift = pin * 4;
        port->AFRL &= ~(0xFU << shift);     
        port->AFRL |= (AFR << shift);   
    }
    else{
        uint32_t shift = (pin-8) * 4;
        port->AFRH &= ~(0xFU << shift);    
        port->AFRH |= (AFR << shift);   
    }
}

void gpio_output_type(GPIO_TypeDef *port, uint8_t pin, GPIO_OutputType type){
    port->OTYPER &= ~(1U << pin);
    port->OTYPER |= ((uint32_t)type << pin);
}

void gpio_speed(GPIO_TypeDef *port, uint8_t pin, GPIO_Speed speed)
{
    port->OSPEEDR &= ~(0x3U << (pin * 2));
    port->OSPEEDR |= ((uint32_t)speed << (pin * 2));
}

void gpio_pull(GPIO_TypeDef *port, uint8_t pin, GPIO_Pull pull)
{
    port->PUPDR &= ~(0x3U << (pin * 2));
    port->PUPDR |= ((uint32_t)pull << (pin * 2));
}

void gpio_write(GPIO_TypeDef *port, uint32_t pin, uint32_t value){
    if(value){
        port->BSRR |= (1U << pin);
    }
    else{
        port->BSRR |= (1U << (pin + 16));
    }
}

uint32_t gpio_read(GPIO_TypeDef *port, uint32_t pin){
    return (port->IDR >> pin) & 1;
}

void gpio_toggle(GPIO_TypeDef *port, uint32_t pin){
    if(port->ODR & (1U << pin)){
        port->BSRR = (1U << (pin + 16));        
    }
    else{
        port->BSRR = (1U << pin);
    }
}

