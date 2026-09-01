#include "rtos.h"


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

    for (;;){
        imu_read();

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        int correction = imu_pid();

        motor_set_speed(&left_motor, correction);
        motor_set_speed(&right_motor, -correction);

        vTaskDelayUntil(&last_wake_time, pdMS_TO_TICKS(10));

    }
}

void rtos_init(void){
    xTaskCreate(imu_task,"IMU",256,NULL,1,NULL);
}

