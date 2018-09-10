
#ifndef MT_TOKEN_H
#define MT_TOKEN_H

#include <stdint.h>
#include "common.h"
#include "var.h"
#include "buf.h"

#define mt_token(NAME) mt_pfx(_T_##NAME)

#define MT_TOKEN_DATA_CHAR_BUF_SIZE 201

typedef union {
    int64_t mt_int;
    double mt_float;
    mt_buf* mt_var;
} mt_token_data;

typedef enum {
    mt_token(VAR) = 128,
    mt_token(INT) = 129,
    mt_token(FLOAT) = 130,
    mt_token(ASSIGN) = ':',
    mt_token(L_BRACE) = '(',
    mt_token(R_BRACE) = ')',
    mt_token(COMMA) = ',',
    mt_token(QUESTION) = '?',
    mt_token(EQ) = '=',
    mt_token(OR) = '|',
    mt_token(L_BRACKET) = '{',
    mt_token(R_BRACKET) = '}',
    mt_token(DOLLAR) = '$',
    mt_token(ADD) = '+',
    mt_token(SUB) = '-',
    mt_token(GREATER) = '>',
    mt_token(WRITE) = 131,
    mt_token(SLASH) = '/',
    mt_token(NL) = '\n',
    mt_token(END) = ';'
} mt_token_type;

typedef struct _mt_token {
    mt_token_type type;
    mt_token_data data;
    size_t line, c;
    struct _mt_token* next;
} mt_token;

#define mt_token_state(NAME) mt_pfx(_T_S_##NAME)

typedef struct {
    enum {
        mt_token_state(NOTHING),
        mt_token_state(COMMENT),
        mt_token_state(VAR),
        mt_token_state(INT),
        // mt_token_state(FLOAT)
    } state;
    size_t line, c;
    mt_buf_iter iter;
    mt_token* head;
    mt_token* tail;
    mt_buf* cur_data;
} mt_token_state;

void mt_token_state_init(mt_token_state* const state);

void mt_token_state_free(mt_token_state* const state);

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf);

void mt_token_state_debug_print(const mt_token_state* const state);

#endif
