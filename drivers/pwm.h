#ifndef PWM_H
#define PWM_H
#include "common.h"
#include "tim.h"

typedef enum {
    PWM_CHANNEL_1,
    PWM_CHANNEL_2,
    PWM_CHANNEL_3,
    PWM_CHANNEL_4
} PWM_Channel_t;

typedef enum
{
    CCxS_OUTPUT    = 0,
    CCxS_INPUT_TIx = 1,
    CCxS_INPUT_TIy = 2,
    CCxS_INPUT_TRC = 3
} CCxS_t;

typedef enum
{
    OCM_FROZEN            = 0,
    OCM_ACTIVE_ON_MATCH   = 1,
    OCM_INACTIVE_ON_MATCH = 2,
    OCM_TOGGLE            = 3,
    OCM_FORCE_INACTIVE    = 4,
    OCM_FORCE_ACTIVE      = 5,
    OCM_PWM_MODE_1        = 6,
    OCM_PWM_MODE_2        = 7
} OCM_t;

void pwm_init(TIM2_5_TypeDef *timer, PWM_Channel_t channel);
void pwm_duty_set(TIM2_5_TypeDef *timer, PWM_Channel_t channel, uint32_t CCR);
void pwm_start(TIM2_5_TypeDef *timer);
void pwm_stop(TIM2_5_TypeDef *timer);

#endif

