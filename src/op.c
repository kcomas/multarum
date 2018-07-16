
#include "op.h"

#define qname(OP) op_str[MT_##OP] = #OP

static char* op_str[_MT_OP_TOTAL];

static bool op_str_init = false;

void mt_op_str_init(void) {
    if (op_str_init) {
        return;
    }
    qname(NOP);
    qname(PUSH);
    qname(ADD);
    qname(SUB);
    qname(JMP);
    qname(LD_SELF),
    qname(CALL);
    qname(RET);
    qname(HALT);
    op_str_init = true;
}

const char* mt_op_str(mt_op op) {
    return op_str[op];
}
