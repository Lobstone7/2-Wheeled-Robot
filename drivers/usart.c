#include "usart.h"

C_Buffer rx_buffer = {
    .head = 0,
    .tail = 0,
};

C_Buffer tx_buffer = {
    .head = 0,
    .tail = 0,
};

static usartcallback usart_cb = 0;

void usart_register_callback(usartcallback cb){
	usart_cb = cb;
}

void usart2_init(){
    
    gpio_init(GPIOA,2,2);                             //Initialize GPIO pins for RX and TX
    gpio_init(GPIOA,3,2);

	GPIOA->AFRL &= ~(0xFU << (2 * 4));                  //Clear field to enable ALt function 7 for pins 2 and 3 
    GPIOA->AFRL &= ~(0xFU << (3 * 4));

    GPIOA->AFRL |= (7U << (2 * 4));                     //ENable Alt Function 7 for pins 2 and 3
    GPIOA->AFRL |= (7U << (3 * 4));

	USART2->BRR = 694;

	USART2->CR1 |= (1U << 2);                          //Receiver Enable
    USART2->CR1 |= (1U << 3);                          //Transmitter Enable
	USART2->CR1 |= (1U << 5);                         //RXNE enable

	USART2->CR1 |= (1U << 0);                          //USART Enable

	NVIC->ISER[1] |= (1U << 6);
   
}

void usart2_write_byte(uint8_t data){
     if (push(&tx_buffer, data)){
        USART2->CR1 |= (1U << 7);
    }
}

uint8_t usart2_read_byte(){
    while(!(USART2->ISR & (1U << 5))); 
    return USART2->RDR;
}

void usart2_write(uint8_t *data, uint32_t length){
    while(length){
        usart2_write_byte(*data);
        data++;
        length--;
    }
}

void usart2_read(uint8_t *buffer,uint32_t length){
    while(length){
        *buffer = usart2_read_byte();
        buffer++;
        length--;
    }
}

void usart2_write_int(int32_t byte){
    char buffer[12];
    uint8_t i = 0;

    if(byte ==0){
        usart2_write_byte('0');
        return;
    }
    if(byte < 0){
        usart2_write_byte('-');
        byte = -byte;
    }

    while(byte > 0){
        buffer[i++] = byte %10 + '0';
        byte /= 10;
    }

    while(i>0){
        usart2_write_byte(buffer[--i]);
    }

}

void usart2_write_hex(uint8_t value){
    char hex[] = "0123456789ABCDEF";

    usart2_write_byte(hex[(value >> 4) & 0x0F]);
    usart2_write_byte(hex[value & 0x0F]);
}

void usart2_write_float(float value){
    if (value < 0.0f)
    {
        usart2_write_byte('-');
        value = -value;
    }

    int32_t integer = (int32_t)value;
    int32_t fraction = (int32_t)((value - integer) * 1000.0f + 0.5f);

    usart2_write_int(integer);
    usart2_write_byte('.');

    if (fraction < 100)
        usart2_write_byte('0');
    if (fraction < 10)
        usart2_write_byte('0');

    usart2_write_int(fraction);
}

bool usart2_read_byte_bool(uint8_t *data){
    return pop(&rx_buffer,data);
}

void USART2_Handler(void){

	if(USART2->ISR & (1U << 5)){
		uint8_t c = (uint8_t)(USART2->RDR);
		push(&rx_buffer,c);
	}

    if (USART2->ISR & (1U << 7)){
        uint8_t c;

        if (pop(&tx_buffer, &c)){
            USART2->TDR = c;
        }
        else{
            USART2->CR1 &= ~(1U << 7);
        }
    }
}