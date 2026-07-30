#ifndef COMMON_H
#define COMMON_H

#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include <string.h>

#define PERIPHERAL_BASE (0x40000000U)

#define APB1_OFFSET (0x00)
#define APB1 (PERIPHERAL_BASE + APB1_OFFSET)

#define APB2_OFFSET (0x00010000)
#define APB2 (PERIPHERAL_BASE + APB2_OFFSET)

#define AHB1_OFFSET (0x00020000U)
#define AHB1 (PERIPHERAL_BASE + AHB1_OFFSET)

#define AHB2_OFFSET (0x08000000U)
#define AHB2 (PERIPHERAL_BASE + AHB2_OFFSET)

#define FLASH_ACR       (*(volatile uint32_t *)(0x40022000))

#define PI         3.14159265f
#define DEG_TO_RAD (PI / 180.0f)
#define RAD_TO_DEG (180.0f / PI)

#endif