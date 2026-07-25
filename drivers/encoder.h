#ifndef ENCODER_H
#define ENCODER_H

#include "common.h"
#include "gpio.h"
#include "exti.h"

typedef struct{
    GPIO_pin C1;
    GPIO_pin C2;
    uint8_t previous_state;
    int32_t count;
}Encoder_s;

typedef enum{
    ENC_00,
    ENC_01,
    ENC_10,
    ENC_11,
}Enc_States;

void encoder_callback(void *ctx);
void encoder_init(Encoder_s* Encoder);
#endif