#include "exti.h"

static exticallback exti_cb[16] = {0};
static void* context[16] = {0};

void exti_register_callback(exticallback cb,uint32_t line,void *ctx){
    exti_cb[line] = cb;
    context[line] = ctx;
}

static const EXTI_IRQn_t exti_irq_table[16] = {
    EXTI0_IRQn,    
    EXTI1_IRQn,    
    EXTI2_IRQn,    
    EXTI3_IRQn,    
    EXTI4_IRQn,    
    EXTI9_5_IRQn,
    EXTI9_5_IRQn, 
    EXTI9_5_IRQn, 
    EXTI9_5_IRQn, 
    EXTI9_5_IRQn, 
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
    EXTI15_10_IRQn,
};

void rcc_exti_enable(){
    RCC_APB2ENR |= (1U << 0);
}

void exti_init(GPIO_pin *pin,Edge_Type trigger){
    uint32_t line = pin->pin;
    uint32_t reg_index = line / 4;
    uint32_t shift = (line % 4) * 4;
    uint32_t port_no = ((uintptr_t)pin->port - GPIOA_BASE) / GPIO_PORT_SPACE;

    (&SYSCFG->EXTICR1)[reg_index] &= ~(port_no << shift);
    (&SYSCFG->EXTICR1)[reg_index] |= (port_no << shift);

    switch(trigger){
        case RISING:
        EXTI->RTSR1 |= (1U << line);
        EXTI->FTSR1 &= ~(0xFU << line);
        break;

        case FALLING:
        EXTI->FTSR1 |= (1U << line);
        EXTI->RTSR1 &= ~(0xFU << line);
        break;

        case BOTH:
        EXTI->RTSR1 |= (1U << line);
        EXTI->FTSR1 |= (1U << line);
        break;
    }

    EXTI->IMR1 |= (1U << line);

    uint32_t irq = exti_irq_table[line];
    uint32_t iser_reg = irq / 32;
    uint32_t bit = irq % 32;
    NVIC->ISER[iser_reg] = (1U << bit);

}

static void exti_line_helper(uint32_t line){
    uint32_t mask = (1U << line);
    if(EXTI->PR1 & mask){
        EXTI->PR1 = mask;
        if(exti_cb[line]){
            exti_cb[line](context[line]);
        }
    }
}

void EXTI0_Handler(void){
    exti_line_helper(0);
}

void EXTI1_Handler(void){
    exti_line_helper(1);
}

void EXTI2_Handler(void){
    exti_line_helper(2);
}

void EXTI3_Handler(void){
    exti_line_helper(3);
}

void EXTI4_Handler(void){
    exti_line_helper(4);
}

void EXTI9_5_Handler(void){
    for(int i = 5;i<=9;i++){
        exti_line_helper(i);
    }
}

void EXTI15_10_Handler(void){
    for(int i = 10;i<=15;i++){
        exti_line_helper(i);
    }
}


