
#ifndef MT_MOD
#define MT_MOD

#include "multarum.h"

typedef struct _mt_mod {
    uint32_t ref_count;
    size_t _size, len, _f_size, f_len;
    uint8_t* bytes;
    uint8_t* fns;
    // @TODO hash exports
} mt_mod;

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

#define mt_mod_copy_inc(mod, i, to, type) \
    memcpy(&to, mod->bytes + i, sizeof(type)); \
    i += sizeof(type)

#define mt_mod_op_w_data(mod, i, type, data, str) \
    mt_print_byte_hex(mod, i, 1 + sizeof(type)); \
    i++; \
    mt_mod_copy_inc(mod, i, data, type); \
    mt_mod_print_even_spaces(1 + sizeof(type)); \
    printf(str, data);

void mt_mod_dis(const mt_mod* const mod);

#endif
