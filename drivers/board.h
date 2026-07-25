#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "clock.h"
#include "gpio.h"
#include "tim.h"
#include "pwm.h"
#include "motor.h"
#include "exti.h"
#include "encoder.h"
#include "usart.h"

#define LEFT_MOTOR_TIMER_PORT GPIOA
#define LEFT_MOTOR_TIMER_PIN 0

#define LEFT_IN1_PORT GPIOA
#define LEFT_IN1_PIN  4

#define LEFT_IN2_PORT GPIOA
#define LEFT_IN2_PIN  6

#define RIGHT_MOTOR_TIMER_PORT GPIOA
#define RIGHT_MOTOR_TIMER_PIN 1

#define RIGHT_IN1_PORT GPIOA
#define RIGHT_IN1_PIN 7

#define RIGHT_IN2_PORT GPIOA
#define RIGHT_IN2_PIN 8

#define TB_STBY_PORT GPIOA
#define TB_STBY_PIN 9

#define LEFT_MOTOR_TIMER TIM2
#define LEFT_MOTOR_CHANNEL PWM_CHANNEL_1

#define RIGHT_MOTOR_TIMER TIM2
#define RIGHT_MOTOR_CHANNEL PWM_CHANNEL_2

#define LEFT_MOTOR_C1_PORT GPIOA
#define LEFT_MOTOR_C1_PIN 11

#define LEFT_MOTOR_C2_PORT GPIOA
#define LEFT_MOTOR_C2_PIN 15

#define RIGHT_MOTOR_C1_PORT GPIOB
#define RIGHT_MOTOR_C1_PIN 4

#define RIGHT_MOTOR_C2_PORT GPIOB
#define RIGHT_MOTOR_C2_PIN 5


extern Motor left_motor;
extern Motor right_motor;

extern Encoder_s left_encoder;
extern Encoder_s right_encoder;

void board_init();

#endif