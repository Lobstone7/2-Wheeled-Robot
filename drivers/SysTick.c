#include "SysTick.h"

static volatile uint32_t tick = 0;

void SysTick_init(){
    SysTick->CVR = 0;
    SysTick->RVR = 79999 & 0x00FFFFFF;
    SysTick->CSR =  (1U << 0) | (1U << 1) | (1U << 2);
}

void SysTick_Handler(void){
    tick++;
}

uint32_t SysTick_get_ms(){
    return tick;
}