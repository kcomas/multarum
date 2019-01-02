
#include "utf8.h"

utf8 utf8_init(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    utf8 u;
    u.b[0] = a;
    u.b[1] = b;
    u.b[2] = c;
    u.b[3] = d;
    return u;
}

utf8 utf8_from_c(char c) {
    utf8 u = (utf8) { .b = { 0, 0, 0, 0 } };
    u.b[0] = (uint8_t) c;
    return u;
}

size_t utf8_len(utf8 c) {
    if ((c.b[0] & 0x80) == 0) return 1;
    if ((c.b[0] & 0xe0) == 0xc0) return 2;
    if ((c.b[0] & 0xf0) == 0xe0) return 3;
    if ((c.b[0] & 0xf8) == 0xf0) return 4;
    return 0;
}

int32_t utf8_value(utf8 c) {
    if ((c.b[0] & 0x80) == 0) return (int32_t) c.b[0];
    if ((c.b[0] & 0xe0) == 0xc0) return  ((c.b[0] & 0x1f) << 6) | c.b[1];
    if ((c.b[0] & 0xf0) == 0xe0) {
        uint32_t r = ((c.b[0] & 0x0f) << 12) | (c.b[1] << 6) | c.b[2];
        if (r < 55296 || r > 57343) return (int32_t) r;
    }
    if ((c.b[0] & 0xf8) == 0xf0) {
        uint32_t r = ((c.b[0] & 0x07) << 18) | (c.b[1] << 12) | (c.b[2] << 6) | c.b[3];
        if (r <= 1114111) return (int32_t) r;
    }
    return -1;
}

void utf8_print(const utf8 c) {
    for (size_t i = 0; i < utf8_len(c); i++) putchar(c.b[i]);
}
