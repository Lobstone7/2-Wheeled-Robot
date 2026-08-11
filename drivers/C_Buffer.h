#ifndef C_BUFFER_H
#define C_BUFFER_H
#include "common.h"

typedef struct{
    uint8_t buffer[64];
    volatile uint8_t head;
    volatile uint8_t tail;
}C_Buffer;

bool isEmpty(C_Buffer *cb);
bool isFull(C_Buffer *cb);
bool push(C_Buffer *cb,uint8_t data);
bool pop(C_Buffer *cb,uint8_t *data);

#endif