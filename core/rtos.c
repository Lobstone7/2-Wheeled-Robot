#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "imu.h"

volatile uint32_t task1_count = 0;
volatile uint32_t task2_count = 0;

static void task1(void *argument){
    (void)argument;

    while (1)
    {
        task1_count++;
    }
}

static void task2(void *argument){
    (void)argument;

    while (1){
        task2_count++;
    }
}

static void imu_complete_callback(IMU_Operation operation,Trans_State state,void *context){
    (void)operation;
    (void)state;

    TaskHandle_t task = (TaskHandle_t) context;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    vTaskNotifyGiveFromISR(task,&xHigherPriorityTaskWoken);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

static void imu_task(void *argument){
    TaskHandle_t imu_task_handle = xTaskGetCurrentTaskHandle();

    imu_set_complete_callback(imu_complete_callback,imu_task_handle);


    imu_init();

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    imu_calibrate();

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    imu_read();

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    TickType_t last_wake_time = xTaskGetTickCount();

    for (;;) {
        imu_read();

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(10));

    }
}

void rtos_init(void){
    xTaskCreate(imu_task,"IMU",256,NULL,2,NULL);
    xTaskCreate(task2,"Task2",256,NULL,2,NULL);
}

