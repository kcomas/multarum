
#ifndef MT_MOD
#define MT_MOD

#include <stdlib.h>
#include <stdint.h>

typedef struct {
    size_t _size, len;
    uint8_t* bytecode;
    // @TODO hash exports
} mt_mod;

mt_mod* mt_mod_init(size_t _size);

void mt_mod_free(mt_mod* const mod);

#endif
