#include "C_Buffer.h"

bool isEmpty(C_Buffer *cb){
    if(cb->head == cb->tail){
        return true;
    }
    return false;
}

bool isFull(C_Buffer *cb){
    if(((cb->head + 1) & (64 - 1)) == cb->tail){
        return true;
    }
    return false;
}

bool push(C_Buffer *cb,uint8_t data){
    if(isFull(cb)){
        return false;
    }
    cb->buffer[cb->head] = data;
    cb->head = (cb->head + 1) & (64 - 1);
    return true;
}

bool pop(C_Buffer *cb,uint8_t *data){
    if(isEmpty(cb)){
        return false;
    }
    *data = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) & (64 -1);
    return true;
}