
#ifndef MT_TOKEN_H
#define MT_TOKEN_H

#include <stdint.h>
#include "common.h"
#include "var.h"
#include "buf.h"

#define mt_token(NAME) mt_pfx(_T_##NAME)

typedef union {
    int64_t mt_int;
    double mt_float;
    mt_buf* mt_var_name;
} mt_token_data;

typedef enum {
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
} mt_token_type;

typedef struct _mt_token {
    mt_token_type type;
    mt_token_data data;
    struct _mt_token* next;
} mt_token;

#define mt_token_state(NAME) mt_pfx(_T_S_##NAME)

#define MT_MAX_BUF_NAME 500

typedef struct {
    enum {
        mt_token_state(NOTHING),
        mt_token_state(MABE_COMMENT),
        mt_token_state(COMMENT),
        mt_token_state(VAR),
        mt_token_state(INT),
        mt_token_state(FLOAT)
    } state;
    mt_buf_iter iter;
    mt_token* head;
    mt_token* tail;
} mt_token_state;

void mt_token_state_init(mt_token_state* const state);

// @TODO
// void mt_token_state_free(mt_token_state* const state);

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf);

#endif
