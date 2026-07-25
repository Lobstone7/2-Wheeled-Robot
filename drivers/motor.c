#include "motor.h"

void motor_stop(Motor* motor){
    gpio_write(motor->In1.port, motor->In1.pin, 1);
    gpio_write(motor->In2.port, motor->In2.pin, 1);

    pwm_duty_set(motor->timer, motor->channel, 0);
}

void motor_init(Motor* motor){
    motor_stop(motor);
}

void motor_set_speed(Motor* motor,int speed){
    if(speed == 0){
        motor_stop(motor);
        return;
    }
    
    if(speed<-100){
        speed = -100;
    }

    if(speed > 100){
        speed = 100;
    }
    
    if(speed < 0){
        gpio_write(motor->In1.port, motor->In1.pin, 0);
        gpio_write(motor->In2.port, motor->In2.pin, 1);
    }
    else{
        gpio_write(motor->In1.port, motor->In1.pin, 1);
        gpio_write(motor->In2.port, motor->In2.pin, 0);
    }


    speed = abs(speed);

    uint32_t arr = motor->timer->ARR;
    uint32_t ccr = (speed * (arr + 1))/100;

    pwm_duty_set(motor->timer, motor->channel, ccr);


}