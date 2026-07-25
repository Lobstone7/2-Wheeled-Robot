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

#define RCC_OFFSET (0x00001000U)
#define RCC (AHB1 + RCC_OFFSET)

#define RCC_CR          (*(volatile uint32_t *)(RCC + 0x00))
#define RCC_ICSCR       (*(volatile uint32_t *)(RCC + 0x04))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC + 0x08))
#define RCC_PLLCFGR     (*(volatile uint32_t *)(RCC + 0x0C))

#define FLASH_ACR       (*(volatile uint32_t *)(0x40022000))

#define RCC_APB1ENR1_OFFSET (0x58)
#define RCC_APB1ENR1 (*(volatile uint32_t*)(RCC + RCC_APB1ENR1_OFFSET))

#define RCC_APB2ENR_OFFSET (0x60)
#define RCC_APB2ENR (*(volatile uint32_t*)(RCC + RCC_APB2ENR_OFFSET))


#endif