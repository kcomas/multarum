
#include "op.h"

static const char* op_str[] = {
    "NOP",
    "PUSH",
    "ADD",
    "SUB",
    "EQ",
    "JMP",
    "JMPF",
    "LD_SELF",
    "LD_FN",
    "LD_ARG",
    "CALL",
    "RET",
    "HALT"
};

const char* mt_op_str(mt_op op) {
    return op_str[op];
}
