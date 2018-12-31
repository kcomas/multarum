
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
    for (size_t i = 4; i > 0; i--) if (c.b[i] > 0) return i;
    return 0;
}
