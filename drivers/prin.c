#include "prin.h"


char *itoa_simple(int32_t value, char *buffer)
{
    char *p = buffer;

    if (value == 0) {
        *p++ = '0';
        *p = '\0';
        return buffer;
    }

    if (value < 0) {
        *p++ = '-';
        value = -value;
    }

    char *start = p;

    while (value > 0) {
        *p++ = (value % 10) + '0';
        value /= 10;
    }

    *p = '\0';

    char *end = p - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    return buffer;
}