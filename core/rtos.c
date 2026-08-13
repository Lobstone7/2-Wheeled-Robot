#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"

volatile uint32_t task1_count = 0;
volatile uint32_t task2_count = 0;

static void task1(void *argument)
{
    (void)argument;

    while (1)
    {
        task1_count++;
    }
}

static void task2(void *argument)
{
    (void)argument;

    while (1)
    {
        task2_count++;
    }
}

void rtos_init(void)
{
    xTaskCreate(task1, "Task1", 128, NULL, 1, NULL);
    xTaskCreate(task2, "Task2", 128, NULL, 1, NULL);
}


