
#include "token.h"

static const char *token_name[] = {
    "NONE",
    "TYPE_INT",
    "FN",
    "NAME",
    "INTEGER",
    "TYPE",
    "LBRACE",
    "RBRACE",
    "RBRACKET",
    "LBRACKET",
    "COLON",
    "COMMA",
    "IF",
    "ELIF",
    "ELSE",
    "RETURN",
    "PRINT",
    "LESS_THAN",
    "LESS_THAN_EQUAL",
    "EQUAL",
    "OR",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "EXP",
    "END"
};

static const char *token_word[] = {
    "int",
    "fn",
    "if",
    "elif",
    "else",
    "return",
    "print",
    "<=",
    "==",
    "||",
    "**"
};

static const token_type token_word_type[] = {
    TOKEN(TYPE_INT),
    TOKEN(FN),
    TOKEN(IF),
    TOKEN(ELIF),
    TOKEN(ELSE),
    TOKEN(RETURN),
    TOKEN(PRINT),
    TOKEN(LESS_THAN_EQUAL),
    TOKEN(EQUAL),
    TOKEN(OR),
    TOKEN(EXP)
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

static inline bool is_hash_ident(token_type tt) {
   for (size_t i = 0; i < TOKEN_WORD_LEN; i++) if (tt == token_word_type[i]) return true;
   return false;
}

static inline size_t token_hash_fn(size_t hash, char c) {
    return ((hash << 5) + hash) + c;
}

#ifndef TOKEN_HASH_BASE
    #define TOKEN_HASH_BASE 5381
#endif

#ifndef TOKEN_HASH_REHASH
    #define TOKEN_HASH_REHASH (TOKEN_WORD_LEN / 2)
#endif


static token_state *set_token_hash(token_state *ts, char **const err) {
    for (size_t i = 0; i < TOKEN_WORD_LEN; i++) {
        const char *w = token_word[i];
        char c;
        size_t hash = TOKEN_HASH_BASE;
        while ((c = *w++)) hash = token_hash_fn(hash, c);
        bool found = false;
        for (size_t rehash = 0; rehash < TOKEN_HASH_REHASH && !found; rehash++) {
            size_t new_hash = (hash + rehash) % TOKEN_WORD_HASH_SIZE;
            if (ts->token_word_hash[new_hash] == TOKEN(NONE)) {
                ts->token_word_hash[new_hash] = token_word_type[i];
                found = true;
            }
        }
        if (!found) {
            free(ts);
            *err = "Token Hash Collision";
            return NULL;
        }
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

static bool set_single_char(token_state* state, token *t, token_type type) {
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
                (state->str[state->last_match_start_idx] >= 'A' && state->str[state->last_match_start_idx] <= 'Z') ||
                state->str[state->last_match_start_idx] == '_') {
                state->last_match_type = TOKEN(NAME);
                continue;
            }
            switch (state->str[state->last_match_start_idx]) {
                case ' ':
                    inc_pos(state);
                    break;
                case ';':
                case '\n':
                    state->last_match_type = TOKEN(END);
                    set_token(state, t);
                    inc_line(state);
                    return true;
                case '(': return set_single_char(state, t, TOKEN(RBRACE));
                case ')': return set_single_char(state, t, TOKEN(LBRACE));
                case '{': return set_single_char(state, t, TOKEN(RBRACKET));
                case '}': return set_single_char(state, t, TOKEN(LBRACKET));
                case ':': return set_single_char(state, t, TOKEN(COLON));
                case ',': return set_single_char(state, t, TOKEN(COMMA));
                case '<':
                    state->last_match_type = TOKEN(LESS_THAN);
                    break;
                case '+': return set_single_char(state, t, TOKEN(ADD));
                case '-': return set_single_char(state, t, TOKEN(SUB));
                case '*':
                    state->last_match_type = TOKEN(MUL);
                    break;
                case '/':
                    state->last_match_type = TOKEN(DIV);
                    break;
                default:
                    break;
            }
            continue;
        }
        size_t hash = TOKEN_HASH_BASE;
        for (size_t i = state->last_match_start_idx; i < state->last_match_end_idx; i++) {
            hash = token_hash_fn(hash, state->str[i]);
        }
        bool found = false;
        for (size_t rehash = 0; rehash < TOKEN_HASH_REHASH && !found; rehash++) {
            token_type tt = state->token_word_hash[(hash + rehash) % TOKEN_WORD_HASH_SIZE];
            if (tt != TOKEN(NONE)) {
                size_t idx = 0;
                while (token_word_type[idx] != tt) idx++;
                const char *name = token_word[idx];
                size_t len = strlen(name) - 1;
                size_t i = state->last_match_start_idx;
                if (state->last_match_end_idx - i < len) continue;
                bool match = true;
                char c;
                while ((c = *name++) && match) if (c != state->str[i++]) match = false;
                if (match) {
                    state->last_match_type = tt;
                    found = true;
                }
            }
        }
        if (found) continue; else if (is_hash_ident(state->last_match_type)) return found_token(state, t);
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
                (state->str[i] < '0' || state->str[i] > '9') &&
                (state->str[i] != '_')) is_name = false;
        }
        if (is_name) {
            state->last_match_type = TOKEN(NAME);
            continue;
        }
        if (state->last_match_type != TOKEN(NONE)) return found_token(state, t);
        if (state->last_match_end_idx - state->last_match_start_idx > 10) {
            *err = "Invalid Token Found";
            return false;
        }
    }
    return false;
}
