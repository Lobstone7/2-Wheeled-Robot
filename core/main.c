#include "main.h"

int main(){

    board_init();
    gpio_write(TB_STBY_PORT, TB_STBY_PIN, 1);

    gpio_write(LEFT_IN1_PORT, LEFT_IN1_PIN, 1);
    gpio_write(LEFT_IN2_PORT, LEFT_IN2_PIN, 0);

    pwm_duty_set(TIM2, PWM_CHANNEL_1, 100);

    motor_set_speed(&left_motor,25);
    motor_set_speed(&right_motor,-25);

    char buffer[64];

    

    while(1){
    /*itoa_simple(left_encoder.count, buffer);
    usart2_write((uint8_t *)"L:", 2);
    usart2_write((uint8_t *)buffer, strlen(buffer));

    usart2_write((uint8_t *)" R:", 3);

    itoa_simple(right_encoder.count, buffer);
    usart2_write((uint8_t *)buffer, strlen(buffer));

    usart2_write((uint8_t *)"\r\n", 2);*/

    }
}