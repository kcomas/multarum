
#ifndef MT_CHAR_H
#define MT_CHAR_H

#include <stdint.h>

typedef struct {
    uint8_t a, b, c, d;
} mt_char;

uint8_t mt_char_cont(uint8_t a);

#endif
