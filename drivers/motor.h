#ifndef MOTOR_H
#define MOTOR_H
#include "common.h"
#include "pwm.h"
#include "gpio.h"

typedef struct{
    TIM2_5_TypeDef* timer;
    PWM_Channel_t channel;
    GPIO_pin In1;
    GPIO_pin In2;
}Motor;


void motor_stop(Motor* motor);
void motor_init(Motor* motor);
void motor_set_speed(Motor* motor,int speed);

#endif