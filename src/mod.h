
#ifndef MT_MOD
#define MT_MOD

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    size_t _size, len;
    uint8_t* bytes;
    // @TODO hash exports
} mt_mod;

#define mt_write_byte(mod, byte) mod->bytes[mod->len++] = byte

#define mt_write_bytes(mod, data, size) \
    memcpy(mod->bytes + mod->len, data, size); \
    mod->len += size

#define mt_mod_tail(mod) mod->bytes[mod->len - 1]

mt_mod* mt_mod_init(size_t _size);

void mt_mod_free(mt_mod* const mod);

#endif
