#ifndef RTOS_H
#define RTOS_H

#include "FreeRTOS.h"
#include "task.h"
#include "gpio.h"
#include "motor.h"
#include "board.h"
#include "imu.h"

void rtos_init(void);


#endif