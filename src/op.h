
#ifndef MT_OP_H
#define MT_OP_H

#include "common.h"
#include "mod.h"

typedef enum {
    mt_pfx(NOP),
    mt_pfx(AL),
    mt_pfx(PUSH),
    mt_pfx(ADD),
    mt_pfx(SUB),
    mt_pfx(EQ),
    mt_pfx(OR),
    mt_pfx(JMP),
    mt_pfx(JMPF),
    mt_pfx(LD_SELF),
    mt_pfx(LD_FN),
    mt_pfx(LD_ARG),
    mt_pfx(LD_LOCAL),
    mt_pfx(SV_LOCAL),
    mt_pfx(CALL),
    mt_pfx(CALL_SELF),
    mt_pfx(RET),
    mt_pfx(WRITE),
    mt_pfx(HALT),
} mt_op;

#define mt_write_jmp(mod, type, mt_jmp) \
    mt_write_byte(mod, type); \
    mt_write_bytes(mod, mt_jmp, sizeof(uint32_t))

const char* mt_op_str(mt_op op);

#endif
