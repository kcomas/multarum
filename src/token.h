
#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define TOKEN(NAME) TOKEN_##NAME

typedef enum {
    TOKEN(NONE),
    TOKEN(FN),
    TOKEN(NAME),
    TOKEN(INTEGER),
    TOKEN(TYPE),
    TOKEN(LBRACE),
    TOKEN(RBRACE),
    TOKEN(RBRACKET),
    TOKEN(LBRACKET),
    TOKEN(COLON),
    TOKEN(IF),
    TOKEN(ELIF),
    TOKEN(ELSE),
    TOKEN(RETURN),
    TOKEN(PRINT),
    TOKEN(LESS_THAN),
    TOKEN(EQUAL),
    TOKEN(OR),
    TOKEN(ADD),
    TOKEN(SUB),
    TOKEN(END)
} token_type;

typedef struct _token {
    token_type type;
    size_t line_idx, char_idx, value_len;
    const char *value;
} token;

#define TOKEN_WORD_LEN 6

#define TOKEN_WORD_HASH_LEN TOKEN_WORD_LEN * 4

typedef struct _token_state {
    token_type last_match_type;
    size_t last_match_start_idx, last_match_end_idx;
    size_t line_idx, char_idx, str_len;
    token_type token_word_hash[TOKEN_WORD_HASH_LEN];
    char str[];
} token_state;

token_state *tokenize_file(const char *filename, char **const err);

bool next_token(token_state *state, token *t, char *err);
