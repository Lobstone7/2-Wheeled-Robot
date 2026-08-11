#include "main.h"

void gpio_toggle(GPIO_TypeDef *port, uint32_t pin){
    if(port->ODR & (1U << pin)){
        port->BSRR |= (1U << (pin + 16));        
    }
    else{
        port->BSRR |= (1U << pin);
    }
}

void delay_ms(uint32_t ms){
    uint32_t start = SysTick_get_ms();
    while(SysTick_get_ms() - start  < ms);
}

int main(){

    board_init();
    SysTick_init();

    IMU_STATE state = {0};
    state.bias = imu_calibrate(I2C1);
    PID_Config config = {
        .Kp = 0.0f,          
        .Ki = 0.0f,          
        .Kd = 0.0f,          
        .integral_limit = 20.0f,
        .output_limit = 100.0f
    };

    PID_State p_state = {
        .integral = 0.0f,
        .desired_angle = 0.0f
    };
    
    

    uint32_t previous_ms = SysTick_get_ms();

    while (1){
        imu_update(I2C1,&state);

        uint32_t current_ms = SysTick_get_ms();
        float dt = (current_ms - previous_ms)/1000.0f;
        previous_ms = current_ms;

        int output = PID(&config, &p_state, state.pitch, dt, state.gyro_x);

        motor_set_speed(&left_motor,output);
        motor_set_speed(&right_motor,output);
    }


} 

