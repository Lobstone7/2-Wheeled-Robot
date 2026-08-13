#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configCPU_CLOCK_HZ                 80000000UL
#define configTICK_RATE_HZ                1000
#define configTICK_TYPE_WIDTH_IN_BITS     TICK_TYPE_WIDTH_32_BITS
#define INCLUDE_vTaskDelay    1

#define configMINIMAL_STACK_SIZE          128
#define configTOTAL_HEAP_SIZE          (16 * 1024)
#define configMAX_PRIORITIES               5
#define configUSE_PREEMPTION               1
#define configUSE_TIME_SLICING 1

#define configUSE_IDLE_HOOK               0
#define configUSE_TICK_HOOK               0

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 0x50

#endif