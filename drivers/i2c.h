#ifndef I2C_H
#define I2C_H

#include "common.h"
#include "gpio.h"
#include "usart.h"

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t TIMINGR;
    volatile uint32_t TIMEOUTR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t PECR;
    volatile uint32_t RXDR;
    volatile uint32_t TXDR;
} I2C_TypeDef;

#define I2C1_OFFSET (0x5400U)
#define I2C1_BASE (APB1 + I2C1_OFFSET)
#define I2C1 ((I2C_TypeDef*)I2C1_BASE)

#define I2C2_OFFSET (0x5800U)
#define I2C2_BASE (APB1 + I2C2_OFFSET)
#define I2C2 ((I2C_TypeDef*)I2C2_BASE)

#define I2C3_OFFSET (0x5C00U)
#define I2C3_BASE (APB1 + I2C3_OFFSET)
#define I2C3 ((I2C_TypeDef*)I2C3_BASE)

//CR1
#define CR1_PE_Msk  1U

#define CR1_TXIE_Pos     1U
#define CR1_TXIE_Msk     (1U << CR1_TXIE_Pos)

#define CR1_RXIE_Pos     2U
#define CR1_RXIE_Msk     (1U << CR1_RXIE_Pos)

#define CR1_NACKIE_Pos 4U
#define CR1_NACKIE_Msk (1U << CR1_NACKIE_Pos)

#define CR1_STOPIE_Pos   5U
#define CR1_STOPIE_Msk   (1U << CR1_STOPIE_Pos)

#define CR1_TCIE_Pos     6U
#define CR1_TCIE_Msk     (1U << CR1_TCIE_Pos)

#define CR1_ERRIE_Pos 7U
#define CR1_ERRIE_Msk (1U << CR1_ERRIE_Pos)



//CR2
#define CR2_SADD_Msk 0x3FFU

#define CR2_RD_WRN_Pos 10U
#define CR2_RD_WRN_Msk (1U << CR2_RD_WRN_Pos)

#define CR2_START_Pos 13U
#define CR2_START_Msk (1U << CR2_START_Pos)

#define CR2_STOP_Pos 14U
#define CR2_STOP_Msk (1U << CR2_STOP_Pos)

#define CR2_NBYTES_Pos  16U
#define CR2_NBYTES_Msk  (0xFFU << CR2_NBYTES_Pos)

#define CR2_AUTOEND_Pos 25U
#define CR2_AUTOEND_Msk (1U << CR2_AUTOEND_Pos) 

//ISR
#define ISR_TXIS_Pos     1U
#define ISR_TXIS_Msk     (1U << ISR_TXIS_Pos)

#define ISR_RXNE_Pos     2U
#define ISR_RXNE_Msk     (1U << ISR_RXNE_Pos)

#define ISR_NACKF_Pos    4U
#define ISR_NACKF_Msk    (1U << ISR_NACKF_Pos)

#define ISR_STOPF_Pos    5U
#define ISR_STOPF_Msk    (1U << ISR_STOPF_Pos)

#define ISR_TC_Pos       6U
#define ISR_TC_Msk       (1U << ISR_TC_Pos)

#define ISR_BERR_Pos     8U
#define ISR_BERR_Msk     (1U << ISR_BERR_Pos)

#define ISR_ARLO_Pos     9U
#define ISR_ARLO_Msk     (1U << ISR_ARLO_Pos)

#define ISR_OVR_Pos      10U
#define ISR_OVR_Msk      (1U << ISR_OVR_Pos)

//ICR
#define ICR_NACKCF_Pos   4U
#define ICR_NACKCF_Msk   (1U << ICR_NACKCF_Pos)

#define ICR_STOPCF_Pos   5U
#define ICR_STOPCF_Msk   (1U << ICR_STOPCF_Pos)

#define ICR_BERRCF_Pos   8U
#define ICR_BERRCF_Msk   (1U << ICR_BERRCF_Pos)

#define ICR_ARLOCF_Pos   9U
#define ICR_ARLOCF_Msk   (1U << ICR_ARLOCF_Pos)

#define ICR_OVRCF_Pos    10U
#define ICR_OVRCF_Msk    (1U << ICR_OVRCF_Pos)


typedef enum{
    SUCCESS,
    BUSY,
    NACK,
    BERR,
}Trans_State;

typedef enum{
    WRITE_REG,
    WRITE_DATA,
    READ_REG,
    READ_DATA,
}Trans_Phase;

typedef enum{
    STARTED,
    ACTIVE,
    ERROR,
}Function_Result;




void i2c1_init();
Function_Result i2c_write(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t data,void (*callback)(Trans_State result, void *context), void *context);
Function_Result i2c_read_bytes(I2C_TypeDef *I2C, uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t length,void (*callback)(Trans_State result, void *context), void *context);
Trans_State i2c_get_state(void);
uint8_t i2c_get_data(void);

#endif