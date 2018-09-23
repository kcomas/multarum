
#include "char.h"

int8_t mt_char_cont(uint8_t a) {
    static const uint8_t cmps[] = { 0x80, 0xe0, 0xf0, 0xf8 };
    static const uint8_t eqs[] = { 0, 0xc0, 0xe0, 0xf0 };
    for (uint8_t i = 0; i < 4; i++) {
        if ((a & cmps[i]) == eqs[i]) {
            return i;
        }
    }
    return -1;
}

int32_t mt_char_to_int(mt_char c) {
    int8_t conts = mt_char_cont(c.a);
    switch (conts) {
        case 0:
            return c.a;
        case 1:
            return ((c.a & 0x1f) << 6) | c.b;
        case 2:
            return ((c.a & 0x0f) << 12) | (c.b << 6) | c.c;
        case 3:
            return ((c.a & 0x07) << 18) | (c.b << 12) | (c.c << 6) | c.d;
        default:
            return conts;
    }
}
