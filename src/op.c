
#include "op.h"

static const char* op_str[] = {
    "NOP",
    "AL",
    "FL",
    "PUSH",
    "ADD",
    "SUB",
    "EQ",
    "OR",
    "JMP",
    "JMPF",
    "LD_SELF",
    "LD_FN",
    "LD_ARG",
    "LD_LOCAL",
    "SV_LOCAL",
    "CALL",
    "CALL_SELF",
    "RET",
    "WRITE",
    "HALT"
};

const char* mt_op_str(mt_op op) {
    return op_str[op];
}
