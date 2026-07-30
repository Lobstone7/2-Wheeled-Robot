#include "main.h"

void gpio_toggle(GPIO_TypeDef *port, uint32_t pin){
    if(port->ODR & (1U << pin)){
        port->BSRR |= (1U << (pin + 16));        
    }
    else{
        port->BSRR |= (1U << pin);
    }
}

int main(){

    board_init();
    SysTick_init();
    uint32_t start = SysTick_get_ms();
    gpio_init(GPIOA,5,OUTPUT_MODE);

    

    while(1){
    //GYRO_Bias bias = imu_calibrate(I2C1);
    //IMU_DataF data = imu_read(I2C1, &bias);
//
    //usart2_write((uint8_t *)"AX:", 3);
    //usart2_write_float(data.accel.x);
    //
    //usart2_write((uint8_t *)" AY:", 4);
    //usart2_write_float(data.accel.y);
    //
    //usart2_write((uint8_t *)" AZ:", 4);
    //usart2_write_float(data.accel.z);
    //
    //usart2_write((uint8_t *)" GX:", 4);
    //usart2_write_float(data.gyro.x);
    //
    //usart2_write((uint8_t *)" GY:", 4);
    //usart2_write_float(data.gyro.y);
    //
    //usart2_write((uint8_t *)" GZ:", 4);
    //usart2_write_float(data.gyro.z);
    
    //usart2_write((uint8_t *)"\r\n", 2);

    uint32_t current = SysTick_get_ms();


    if(current - start >= 5000){
        usart2_write_int(current);
        usart2_write((uint8_t *)"\r\n", 2);
        start = current;
    }

    }
}