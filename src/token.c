
#include "token.h"

static const char *token_name[] = {
    "NONE",
    "FN",
    "NAME",
    "INTEGER",
    "TYPE",
    "LBRACE",
    "RBRACE",
    "RBRACKET",
    "LBRACKET",
    "COLON",
    "IF",
    "ELIF",
    "ELSE",
    "RETURN",
    "PRINT",
    "LESS_THAN",
    "EQUAL",
    "OR",
    "ADD",
    "SUB",
    "END"
};

static const char *token_word[] = {
    "fn",
    "if",
    "elif",
    "else",
    "return",
    "print"
};

static const token_type token_word_type[] = {
    TOKEN(FN),
    TOKEN(IF),
    TOKEN(ELIF),
    TOKEN(ELSE),
    TOKEN(RETURN),
    TOKEN(PRINT)
};

static token_state *set_token_hash(token_state *ts, char **const err) {
    for (size_t i = 0; i < TOKEN_WORD_LEN; i++) {
        const char *w = token_word[i];
        char c;
        size_t hash = 5381;
        while ((c = *w++)) hash = ((hash << 5) + hash) + c;
        hash %= TOKEN_WORD_HASH_SIZE;
        if (ts->token_word_hash[hash] != 0) {
            free(ts);
            *err = "Token Hash Collision";
            return NULL;
        }
        ts->token_word_hash[hash] = token_word_type[i];
    }
    return ts;
}

token_state *tokenize_file(const char *filename, char **const err) {
    struct stat s;
    if (stat(filename, &s) == -1) {
        *err = "File Not Found";
        return NULL;
    }
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1) {
        *err = "Cannot Open File";
        return NULL;
    }
    token_state *ts = calloc(1, sizeof(token_state) + sizeof(char) * s.st_size + 1);
    if (read(fd, ts->str, s.st_size) != s.st_size) {
        free(ts);
        *err = "Unable To Read File";
        return NULL;
    }
    ts->str_len = s.st_size;
    return set_token_hash(ts, err);
}
