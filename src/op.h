
#ifndef MT_OP
#define MT_OP

#include "multarum.h"

typedef enum {
    MT_NOP,
    MT_PUSH,
    MT_ADD,
    MT_SUB,
    MT_JMP,
    MT_CALL,
    MT_RET,
    MT_HALT,
    _MT_OP_TOTAL
} mt_op;

#define mt_write_jmp(mod, mt_jmp) \
    mt_write_byte(mod, MT_JMP); \
    memcpy(mod->bytes + mod->len, mt_jmp, sizeof(int32_t)); \
    mod->len += sizeof(int32_t)

void mt_op_str_init(void);

const char* mt_op_str(mt_op op);

#endif
