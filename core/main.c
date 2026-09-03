#include "main.h"

void delay_ms(uint32_t ms){
    uint32_t start = SysTick_get_ms();
    while(SysTick_get_ms() - start  < ms);
}

void i2c_test_callback(Trans_State result, void *context)
{
    (void)result;
    (void)context;

    __asm volatile ("nop");
}

static volatile int imu_done = 0;
static volatile Trans_State imu_result;

static void imu_complete_callback(IMU_Operation operation,Trans_State state,void *context)
{
    (void)operation;
    (void)context;

    imu_result = state;
    imu_done = 1;
}

void main(){

    board_init();
    
    rtos_init();
    vTaskStartScheduler();
    
   

} 

