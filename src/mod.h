
#ifndef MT_MOD
#define MT_MOD

#include "multarum.h"

typedef struct _mt_mod {
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

#define mt_mod_dist_op(mod, i) printf("0x%x %s\n", mod->bytes[i], mt_op_str(mod->bytes[i]));

#define mt_mod_get_bytes(mod, dest, size, i) \
    i++; \
    memcpy(dest, mod->bytes + i, size); \
    i += size

#define mt_mod_write_jmp(mod, name, i, dest) \
    mt_print_byte_hex(mod, i, 5); \
    i++; \
    memcpy(&mt_jmp, mod->bytes + i, sizeof(uint32_t)); \
    i += sizeof(uint32_t); \
    printf("%s %d (%lu:)\n", name, mt_jmp, dest)

void mt_mod_dis(const mt_mod* const mod);

#endif
