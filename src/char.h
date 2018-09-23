
#ifndef MT_CHAR_H
#define MT_CHAR_H

#include <stdint.h>

typedef struct {
    uint8_t a, b, c, d;
} mt_char;

#define mt_char_init(A, B, C, D) (mt_char) { .a = A, .b = B, .c = C, .d = D }

int8_t mt_char_cont(uint8_t a);

int32_t mt_char_to_int(mt_char c);

#endif
