
#ifndef MT_TOKEN
#define MT_TOKEN

#include <stdint.h>
#include "common.h"
#include "buf.h"

typedef struct {
    enum {
        mt_pfx(VAR),
        mt_pfx(TINT),
        mt_pfx(TFLOAT),
        mt_pfx(ASSIGN), // :
        mt_pfx(L_BRACE), // (
        mt_pfx(R_BRACE), // )
        mt_pfx(COMMA),
        mt_pfx(FN_ENTRY), // ) {
        mt_pfx(IF_ENTRY), // ? {
        mt_pfx(EQ), // =
        mt_pfx(OR), // |
        mt_pfx(R_BRACKET), // {
        mt_pfx(L_BRACKET), // }
        mt_pfx(SELF), // $
        mt_pfx(ADD), // +
        mt_pfx(SUB), // -
        mt_pfx(WRITE) // >>
    } type;
    union {
        int64_t mt_int;
        double mt_float;
        mt_buf* mt_var_name;
    } data;
} mt_token;

#endif
