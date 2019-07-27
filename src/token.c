
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
    "print",
    "==",
    "||"
};

static const token_type token_word_type[] = {
    TOKEN(FN),
    TOKEN(IF),
    TOKEN(ELIF),
    TOKEN(ELSE),
    TOKEN(RETURN),
    TOKEN(PRINT),
    TOKEN(EQUAL),
    TOKEN(OR)
};

void print_token(token *t) {
    printf("%s,", token_name[t->type]);
    printf(" line: %lu, char: %lu, len: %lu, ", t->line_idx, t->char_idx, t->value_len);
    for (size_t i = 0; i < t->value_len; i++) {
        switch (t->value[i]) {
            case '\n':
                printf("\\n");
                break;
            default:
                putchar(t->value[i]);
                break;
        }
    }
    putchar('\n');
}

static bool is_hash_ident(token_type tt) {
    switch (tt) {
        case TOKEN(FN):
        case TOKEN(IF):
        case TOKEN(ELIF):
        case TOKEN(ELSE):
        case TOKEN(RETURN):
        case TOKEN(PRINT):
        case TOKEN(EQUAL):
        case TOKEN(OR):
            return true;
    default:
        return false;
    }
}

static inline size_t token_hash_fn(size_t hash, char c) {
    return ((hash << 5) + hash) + c;
}

#ifndef TOKEN_HASH_BASE
    #define TOKEN_HASH_BASE 5381
#endif

static token_state *set_token_hash(token_state *ts, char **const err) {
    for (size_t i = 0; i < TOKEN_WORD_LEN; i++) {
        const char *w = token_word[i];
        char c;
        size_t hash = TOKEN_HASH_BASE;
        while ((c = *w++)) hash = token_hash_fn(hash, c);
        hash %= TOKEN_WORD_HASH_SIZE;
        if (ts->token_word_hash[hash] != TOKEN(NONE)) {
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
    close(fd);
    return set_token_hash(ts, err);
}

static inline void inc_pos(token_state *state) {
    state->char_idx += state->last_match_end_idx - state->last_match_start_idx;
    state->last_match_start_idx = state->last_match_end_idx;
}

static inline void inc_line(token_state *state) {
    inc_pos(state);
    state->char_idx = 0;
    state->line_idx++;
}

static inline void set_token(token_state *state, token *t) {
    t->type = state->last_match_type;
    t->line_idx = state->line_idx;
    t->char_idx = state->char_idx;
    t->value = &state->str[state->last_match_start_idx];
    t->value_len = state->last_match_end_idx - state->last_match_start_idx;
}

static inline bool set_single_char(token_state* state, token *t, token_type type, char **const err) {
    if (state->last_match_type != TOKEN(NONE)) {
        state->last_match_end_idx--;;
        state->last_match_start_idx = state->last_match_end_idx;
        set_token(state, t);
        inc_pos(state);
        return true;
    }
    if (state->last_match_type == TOKEN(NONE) && state->last_match_end_idx - state->last_match_start_idx > 1) {
        *err = "Invalid Token Found";
        return false;
    }
    state->last_match_type = type;
    set_token(state, t);
    inc_pos(state);
    return true;
}

static inline bool found_token(token_state *state, token *t) {
    state->last_match_end_idx--;
    set_token(state, t);
    inc_pos(state);
    return true;
}

bool next_token(token_state *state, token *t, char **const err) {
    state->last_match_type = TOKEN(NONE);
    while (state->last_match_start_idx < state->str_len) {
        state->last_match_end_idx++;
        if (state->last_match_end_idx - state->last_match_start_idx == 1) {
            if (state->str[state->last_match_start_idx] >= '0' && state->str[state->last_match_start_idx] <= '9') {
                state->last_match_type = TOKEN(INTEGER);
                continue;
            }
            if ((state->str[state->last_match_start_idx] >= 'a' && state->str[state->last_match_start_idx] <= 'z') ||
                (state->str[state->last_match_start_idx] >= 'A' && state->str[state->last_match_start_idx] <= 'Z')) {
                state->last_match_type = TOKEN(NAME);
                continue;
            }
            switch (state->str[state->last_match_start_idx]) {
                case ' ':
                    inc_pos(state);
                    break;
                case '\n':
                    state->last_match_type = TOKEN(END);
                    set_token(state, t);
                    inc_line(state);
                    return true;
                case '(': return set_single_char(state, t, TOKEN(RBRACE), err);
                case ')': return set_single_char(state, t, TOKEN(LBRACE), err);
                case '{': return set_single_char(state, t, TOKEN(RBRACKET), err);
                case '}': return set_single_char(state, t, TOKEN(LBRACKET), err);
                case ':': return set_single_char(state, t, TOKEN(COLON), err);
                case '<': return set_single_char(state, t, TOKEN(LESS_THAN), err);
                case '+': return set_single_char(state, t, TOKEN(ADD), err);
                case '-': return set_single_char(state, t, TOKEN(SUB), err);
                default:
                    break;
            }
            continue;
        }
        if (state->str[state->last_match_end_idx - 1] == ' ') {
            if (state->last_match_type == TOKEN(NONE)) {
                *err = "Invalid Token Found";
                return false;
            }
            return found_token(state, t);
        }
        size_t hash = TOKEN_HASH_BASE;
        for (size_t i = state->last_match_start_idx; i < state->last_match_end_idx; i++) {
            hash = token_hash_fn(hash, state->str[i]);
        }
        hash %= TOKEN_WORD_HASH_SIZE;
        token_type tt = state->token_word_hash[hash];
        if (tt != TOKEN(NONE)) {
            size_t idx = 0;
            while (token_word_type[idx] != tt) idx++;
            const char *name = token_word[idx];
            bool match = true;
            size_t i = state->last_match_start_idx;
            char c;
            while ((c = *name++) && match) {
                if (c != state->str[i]) match = false;
                i++;
            }
            if (match) {
                state->last_match_type = tt;
                continue;
            }
            if (!match && is_hash_ident(state->last_match_type)) return found_token(state, t);
        }
        bool is_int = true;
        for (size_t i = state->last_match_start_idx; i < state->last_match_end_idx && is_int; i++) {
            if (!(state->str[i] >= '0' && state->str[i] <= '9')) is_int = false;
        }
        bool is_name = true;
        if (is_int) {
            state->last_match_type = TOKEN(INTEGER);
            continue;
        }
        for (size_t i = state->last_match_start_idx; i < state->last_match_end_idx && is_name; i++) {
            if ((state->str[i] < 'A' || state->str[i] > 'Z') &&
                    (state->str[i] < 'a' || state->str[i] > 'z') &&
                    (state->str[i] < '0' || state->str[i] > '9')) is_name = false;
        }
        if (is_name) {
            state->last_match_type = TOKEN(NAME);
            continue;
        }
        if (state->last_match_type != TOKEN(NONE)) return found_token(state, t);
    }
    return false;
}
