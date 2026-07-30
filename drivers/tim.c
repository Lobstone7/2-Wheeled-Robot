#include "tim.h"

void tim2_5_init(TIM2_5_TypeDef* timer,uint32_t PSC, uint32_t ARR){
    timer->CR1 &= ~TIM_CR1_CEN;
    
    timer->PSC = PSC;
    timer->ARR = ARR;
    timer->CNT = 0;
    timer->EGR |= TIM_EGR_UG;

    timer->SR &= ~TIM_SR_UIF; 

    timer->CR1 |= TIM_CR1_ARPE;

}

void tim_start(TIM2_5_TypeDef* timer){
    timer->CR1 |= TIM_CR1_CEN;
}

void tim_stop(TIM2_5_TypeDef* timer){
    timer->CR1 &= ~TIM_CR1_CEN;
}

void reset_cnt(TIM2_5_TypeDef* timer){
    timer->CNT = 0;
}

uint32_t read_cnt(TIM2_5_TypeDef* timer){
    return timer->CNT;
}

void tim_update(TIM2_5_TypeDef* timer){
    timer->EGR |= TIM_EGR_UG;
}