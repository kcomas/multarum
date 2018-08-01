
#ifndef MT_MOD
#define MT_MOD

#include "common.h"
#include "var.h"
#include "op.h"

typedef struct _mt_mod {
    uint8_t _f_size, f_len;
    uint32_t ref_count;
    uint32_t _size, len;
    uint8_t* bytes;
    uint8_t* fns;
    // @TODO hash exports
} mt_mod;

typedef struct {
    bool in;
    uint8_t idx;
} mt_fn_place;

#define mt_write_byte(mod, byte) mod->bytes[mod->len++] = byte

#define mt_write_bytes(mod, data, size) \
    memcpy(mod->bytes + mod->len, data, size); \
    mod->len += size

#define mt_mod_tail(mod) mod->bytes[mod->len - 1]

mt_mod* mt_mod_init(size_t _size, size_t _f_size);

#define mt_mod_reg_fn(mod, i) mod->fns[mod->f_len++] = i

void mt_mod_free(mt_mod* const mod);

#define mt_mod_get_bytes(mod, dest, size, i) \
    i++; \
    memcpy(dest, mod->bytes + i, size); \
    i += size

#define MT_MOD_MAX_BYTES_LINE 10

#define MT_MOD_BYTE_PRINT_LINE 3

void mt_mod_dis(const mt_mod* const mod);

#endif
