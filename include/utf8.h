
#ifndef UTF8_H
#define UTF8_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t b[4]; // byte
} utf8;

utf8 utf8_init(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

utf8 utf8_from_c(char c);

bool utf8_cmp(utf8 a, utf8 b);

size_t utf8_cont(uint8_t c);

size_t utf8_len(utf8 c);

int32_t utf8_value(utf8 c);

void utf8_print(const utf8 c);

#endif
