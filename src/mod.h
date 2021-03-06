
#ifndef MT_MOD_H
#define MT_MOD_H

#include "common.h"
#include "var.h"
#include "op.h"

typedef struct _mt_hash mt_hash;

typedef struct _mt_mod {
    uint8_t _f_size, f_len;
    uint32_t ref_count;
    uint16_t _size, len;
    uint8_t* bytes;
    uint16_t* fns;
    uint16_t* fne;
    mt_hash* globals;
} mt_mod;

#define mt_write_byte(mod, byte) mod->bytes[mod->len++] = byte

#define mt_write_bytes(mod, data, size) \
    memcpy(mod->bytes + mod->len, data, size); \
    mod->len += size

#define mt_mod_tail(mod) mod->bytes[mod->len - 1]

#define MT_MOD_GLOBAL_BUCKETS 20

#define MT_MOD_DEFAULT_SIZE UINT16_MAX // prevents massive files

#define MT_MOD_DEFAULT_FN_SIZE UINT8_MAX

mt_mod* mt_mod_init(uint16_t _size, uint8_t _f_size);

#define mt_mod_reg_fn(mod, i) mod->fns[mod->f_len++] = i

#define mt_mod_reg_fne(mod, f_idx, i) mod->fne[f_idx] = i

void mt_mod_free(mt_mod* const mod);

#define mt_mod_get_bytes(mod, dest, size, i) \
    i++; \
    memcpy(dest, mod->bytes + i, size); \
    i += size

#define MT_MOD_MAX_BYTES_LINE 10

#define MT_MOD_BYTE_PRINT_LINE 3

void mt_mod_dis(const mt_mod* const mod);

#endif
