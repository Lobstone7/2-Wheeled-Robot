#include "i2c.h"

typedef struct{
    I2C_TypeDef *I2C;

    uint8_t address;
    uint8_t reg;
    uint8_t data;
    uint8_t *buffer;
    uint8_t length;
    uint8_t rx_index;
    Trans_State state;
    Trans_Phase phase;

    void (*callback)(Trans_State state, void *context);
    void *callback_context;
}I2C_Context;


static I2C_Context i2c1_context;

void i2c1_init(){
    I2C1->CR1 &= ~CR1_PE_Msk;

    I2C1->TIMINGR = 0x10909CEC;

    I2C1->CR1 |= CR1_PE_Msk;

    NVIC->ISER[0] |= (1U << 31);

}

Function_Result i2c_write(I2C_TypeDef* I2C,uint8_t address,uint8_t reg,uint8_t data,void (*callback)(Trans_State result, void *context), void *context){

    if(i2c1_context.state == BUSY){
        return ACTIVE;
    }
    else{
        i2c1_context.state = BUSY;
        i2c1_context.I2C = I2C;
        i2c1_context.address = address;
        i2c1_context.reg = reg;
        i2c1_context.data = data;
        i2c1_context.phase = WRITE_REG;

        i2c1_context.callback = callback;
        i2c1_context.callback_context = context;

        I2C->CR2  = 0;
        I2C->CR2 |= ((address << 1) & CR2_SADD_Msk);

        I2C->CR2 &= ~CR2_RD_WRN_Msk;

        I2C->CR2 &= ~CR2_NBYTES_Msk;
        I2C->CR2 |= (2U << CR2_NBYTES_Pos);

        I2C->CR2 |= CR2_AUTOEND_Msk;

        I2C->CR1 |= CR1_TXIE_Msk;
        I2C->CR1 |= CR1_STOPIE_Msk;
        I2C->CR1 |= CR1_NACKIE_Msk;
        I2C->CR1 |= CR1_ERRIE_Msk;

        I2C->CR2 |= CR2_START_Msk;
    }
    return STARTED;

}

Function_Result i2c_read_bytes(I2C_TypeDef *I2C, uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t length,void (*callback)(Trans_State result, void *context), void *context){   
    if(length == 0){
        return ERROR;
    }
    if(i2c1_context.state == BUSY){
        return ACTIVE;
    }
    else{
        i2c1_context.state = BUSY;
        i2c1_context.I2C = I2C;
        i2c1_context.address = address;
        i2c1_context.reg = reg;
        i2c1_context.buffer = buffer;
        i2c1_context.length = length;
        i2c1_context.rx_index = 0;
        i2c1_context.phase = READ_REG;

        i2c1_context.callback = callback;
        i2c1_context.callback_context = context;

        I2C->CR2  = 0;
        I2C->CR2 |= ((address << 1) & CR2_SADD_Msk);

        I2C->CR2 &= ~CR2_RD_WRN_Msk;

        I2C->CR2 &= ~CR2_NBYTES_Msk;
        I2C->CR2 |= (1U << CR2_NBYTES_Pos);

        I2C->CR2 &= ~CR2_AUTOEND_Msk;

        I2C->CR1 |= CR1_TXIE_Msk;
        I2C->CR1 |= CR1_RXIE_Msk;
        I2C->CR1 |= CR1_TCIE_Msk;
        I2C->CR1 |= CR1_STOPIE_Msk;
        I2C->CR1 |= CR1_NACKIE_Msk;
        I2C->CR1 |= CR1_ERRIE_Msk;

        I2C->CR2 |= CR2_START_Msk;



        return STARTED;
    }

}

void I2C1_EV_Handler(void){
    I2C_Context *ctx = &i2c1_context;

    if(ctx->I2C->ISR & ISR_TXIS_Msk){
        if(ctx->phase == WRITE_REG ){
            ctx->I2C->TXDR = ctx->reg;
            ctx->phase = WRITE_DATA;
        }
        else if(ctx->phase == WRITE_DATA){
            ctx->I2C->TXDR = ctx->data;
        }
        else if(ctx->phase == READ_REG){
            ctx->I2C->TXDR = ctx->reg;
        }
    }

    if(ctx->I2C->ISR & ISR_TC_Msk){
       if(ctx->phase == READ_REG){
            ctx->phase = READ_DATA;
            ctx->I2C->CR2 |= CR2_RD_WRN_Msk;
            ctx->I2C->CR2 &= ~CR2_NBYTES_Msk;
            ctx->I2C->CR2 |= ((uint32_t)ctx->length << CR2_NBYTES_Pos);
            ctx->I2C->CR2 |= CR2_AUTOEND_Msk;
            ctx->I2C->CR2 |= CR2_START_Msk;
       }
       
    }

    if (ctx->I2C->ISR & ISR_RXNE_Msk) {
        if (ctx->phase == READ_DATA) {
            ctx->buffer[ctx->rx_index] = ctx->I2C->RXDR;
            ctx->rx_index++;
        }
    }

    if(ctx->I2C->ISR & ISR_STOPF_Msk){
        ctx->state = SUCCESS;
        ctx->I2C->ICR = ICR_STOPCF_Msk;

        ctx->I2C->CR1 &= ~(CR1_TXIE_Msk | CR1_RXIE_Msk | CR1_STOPIE_Msk | CR1_TCIE_Msk | CR1_NACKIE_Msk | CR1_ERRIE_Msk);

        void (*callback)(Trans_State, void *) = ctx->callback;
        void *callback_context = ctx->callback_context;

        ctx->rx_index = 0;
        ctx->buffer = NULL;
        ctx->length = 0;
        ctx->callback = NULL;
        ctx->callback_context = NULL;

        if(callback != NULL){
            callback(ctx->state, callback_context);
        }
        
        return;
    }

    if(ctx->I2C->ISR & ISR_NACKF_Msk){
        ctx->state = NACK;
        ctx->I2C->ICR = ICR_NACKCF_Msk;

        ctx->I2C->CR1 &= ~(CR1_TXIE_Msk | CR1_RXIE_Msk | CR1_STOPIE_Msk | CR1_TCIE_Msk | CR1_NACKIE_Msk | CR1_ERRIE_Msk);

        void (*callback)(Trans_State, void *) = ctx->callback;
        void *callback_context = ctx->callback_context;

        ctx->rx_index = 0;
        ctx->buffer = NULL;
        ctx->length = 0;
        ctx->callback = NULL;
        ctx->callback_context = NULL;

        if(callback != NULL){
            callback(ctx->state, callback_context);
        }
        return;
    }

    if(ctx->I2C->ISR & ISR_BERR_Msk ){
        ctx->state = BERR;
        ctx->I2C->ICR = ICR_BERRCF_Msk;

        ctx->I2C->CR1 &= ~(CR1_TXIE_Msk | CR1_RXIE_Msk | CR1_STOPIE_Msk | CR1_TCIE_Msk | CR1_NACKIE_Msk | CR1_ERRIE_Msk);

        void (*callback)(Trans_State, void *) = ctx->callback;
        void *callback_context = ctx->callback_context;

        ctx->rx_index = 0;
        ctx->buffer = NULL;
        ctx->length = 0;
        ctx->callback = NULL;
        ctx->callback_context = NULL;

        if(callback != NULL){
            callback(ctx->state, callback_context);
        }
        return;
    }
}

Trans_State i2c_get_state(void)
{
    return i2c1_context.state;
}

uint8_t i2c_get_data(void)
{
    return i2c1_context.data;
}