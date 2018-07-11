
#ifndef MT_OP
#define MT_OP

#include "mod.h"

typedef enum {
    MT_NOP,
    MT_PUSH,
    MT_ADD,
    MT_SUB,
    MT_JMP,
    MT_HALT
} mt_op;

#define mt_write_jmp(mod, mt_jmp) \
    mt_write_byte(mod, MT_JMP); \
    memcpy(mod->bytes + mod->len, mt_jmp, sizeof(uint32_t)); \
    mod->len += sizeof(uint32_t)

#endif
