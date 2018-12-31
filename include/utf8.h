
#ifndef UTF8_H
#define UTF8_H

typedef struct {
    uint8_t b[4]; // byte
} utf8;

utf8 utf8_init(uint8_t a, uint8_t b, uint8_t c, uint8_t d);

utf8 utf8_from_c(char c);

#endif
