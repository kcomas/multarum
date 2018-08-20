
#include "char.h"

uint8_t mt_char_cont(uint8_t a) {
    static uint8_t cmps[] = { 0x80, 0xe0, 0xf0, 0xf8 };
    static uint8_t eqs[] = { 0, 0xc0, 0xe0, 0xf0 };
    for (uint8_t i = 0; i < 4; i++) {
        if ((a & cmps[i]) == eqs[i]) {
            return i;
        }
    }
    return 0;
}
