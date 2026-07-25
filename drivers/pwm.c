#include "pwm.h"

void pwm_init(TIM2_5_TypeDef *timer, PWM_Channel_t channel){
    volatile uint32_t *ccmr = NULL;

    if(channel < 2){
        ccmr = &timer->CCMR1; 
    }
    else{
        ccmr = &timer->CCMR2; 
    }

    uint32_t shift = (channel & 1U) ? 8U : 0U;
   
    *ccmr &= ~(0x3U << shift);
    *ccmr |= (CCxS_OUTPUT << shift);

    *ccmr &= ~(0x7U << (shift + 4));
    *ccmr &= ~(1U << (shift + 16));
    *ccmr |= (OCM_PWM_MODE_1 << (shift + 4));

    *ccmr |= (1U << (shift + 3));

    timer->CCER &= ~(1U << (channel * 4 + 1));
    timer->CCER |= (1U << (channel * 4));
    
 
}

void pwm_duty_set(TIM2_5_TypeDef *timer, PWM_Channel_t channel, uint32_t CCR){
    switch(channel){
        case PWM_CHANNEL_1:
        timer->CCR1 = CCR;
        break;

        case PWM_CHANNEL_2:
        timer->CCR2 = CCR;
        break;

        case PWM_CHANNEL_3:
        timer->CCR3 = CCR;
        break;

        case PWM_CHANNEL_4:
         timer->CCR4 = CCR;
        break;
    }
}

void pwm_start(TIM2_5_TypeDef *timer){
    tim_start(timer);
}

void pwm_stop(TIM2_5_TypeDef *timer){
    tim_stop(timer);
}