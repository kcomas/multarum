
#ifndef MT_MOD
#define MT_MOD

#include <stdlib.h>
#include <stdint.h>
#include "var.h"

typedef struct {
    size_t _b_size, b_len, _e_size, e_len;
    uint8_t* bytecode;
    struct {
        char* name;
        mt_var value;
    }* exports;
} mt_mod;

#endif
