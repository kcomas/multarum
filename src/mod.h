
#ifndef MT_MOD
#define MT_MOD

#include <stdlib.h>
#include <stdint.h>

typedef {
    size_t _b_size, b_len, _f_size, f_len;
    uint8_t* bytecode;
    uint8_t** fns;
} mt_mod;

typedef {
    size_t _size, len;
    mt_mod* modules;
} mt_mods;

#endif
