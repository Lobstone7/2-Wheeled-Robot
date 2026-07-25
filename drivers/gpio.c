#include "gpio.h"

void rcc_gpioa_enable(){
    RCC_AHB2ENR |= (1U << 0);
}

void rcc_gpiob_enable(){
    RCC_AHB2ENR |= (1U << 1);
}

void rcc_gpioc_enable(){
    RCC_AHB2ENR |= (1U << 2);
}

void gpio_pupd(GPIO_Typedef *port, uint32_t pin, uint32_t mode){
    port->PUPDR |= (mode << (pin * 2));
}

void gpio_init(GPIO_Typedef *port, uint32_t pin, uint32_t mode){
    port->MODER &= ~(3U << (pin * 2));
    port->MODER |= (mode << (pin * 2));
}

void gpio_alt(GPIO_Typedef *port, uint32_t pin, GPIO_AlternateFunction AFR){
    
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

void gpio_write(GPIO_Typedef *port, uint32_t pin, uint32_t value){
    if(value){
        port->BSRR |= (1U << pin);
    }
    else{
        port->BSRR |= (1U << (pin + 16));
    }
}

uint32_t gpio_read(GPIO_Typedef *port, uint32_t pin){
    return (port->IDR >> pin) & 1;
}

