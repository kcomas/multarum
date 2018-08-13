
#ifndef MT_TOKEN
#define MT_TOKEN

#include <stdint.h>
#include "common.h"
#include "var.h"
#include "buf.h"

#define mt_token(NAME) mt_pfx(_T_##NAME)

typedef struct {
    enum {
        mt_token(VAR),
        mt_token(INT),
        mt_token(FLOAT),
        mt_token(ASSIGN), // :
        mt_token(L_BRACE), // (
        mt_token(R_BRACE), // )
        mt_token(COMMA), // ,
        mt_token(QUESTION), // ?
        mt_token(EQ), // =
        mt_token(OR), // |
        mt_token(R_BRACKET), // {
        mt_token(L_BRACKET), // }
        mt_token(DOLLAR), // $
        mt_token(ADD), // +
        mt_token(SUB), // -
        mt_token(WRITE), // >>
        mt_token(SLASH) // /
    } type;
    union {
        int64_t mt_int;
        double mt_float;
        mt_buf* mt_var_name;
    } data;
} mt_token;

#endif
