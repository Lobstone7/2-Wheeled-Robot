#include "encoder.h"

static const int8_t transition_table[16] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};  

void encoder_callback(void *ctx){
    Encoder_s *encoder = (Encoder_s*) ctx;
    uint8_t C1 = gpio_read(encoder->C1.port,encoder->C1.pin);
    uint8_t C2 = gpio_read(encoder->C2.port,encoder->C2.pin);
    uint8_t current_state = (C1 << 1) | C2;

    uint8_t transition_index = (encoder->previous_state << 2) | current_state;
    int delta = transition_table[transition_index];
    if(delta != 0){
        encoder->count += delta;
        encoder->previous_state = current_state;
    }

}

void encoder_init(Encoder_s* Encoder){
    uint8_t C1 = gpio_read(Encoder->C1.port,Encoder->C1.pin);
    uint8_t C2 = gpio_read(Encoder->C2.port,Encoder->C2.pin);
    Encoder->previous_state = (1U << C1) | C2;
    Encoder->count = 0;

    exti_init(&Encoder->C1,BOTH);
    exti_init(&Encoder->C2,BOTH);

    exti_register_callback(encoder_callback,Encoder->C1.pin, Encoder);
    exti_register_callback(encoder_callback,Encoder->C2.pin, Encoder);

}