
#ifndef MT_OP
#define MT_OP

#include "multarum.h"

typedef enum {
    mt_pfx(NOP),
    mt_pfx(PUSH),
    mt_pfx(ADD),
    mt_pfx(SUB),
    mt_pfx(EQ),
    mt_pfx(JMP),
    mt_pfx(JMPF),
    mt_pfx(LD_SELF),
    mt_pfx(LD_FN),
    mt_pfx(LD_ARG),
    mt_pfx(CALL),
    mt_pfx(RET),
    mt_pfx(HALT),
    _MT_OP_TOTAL
} mt_op;

#define mt_write_jmp(mod, type, mt_jmp) \
    mt_write_byte(mod, type); \
    mt_write_bytes(mod, mt_jmp, sizeof(uint32_t))

void mt_op_str_init(void);

const char* mt_op_str(mt_op op);

#endif
