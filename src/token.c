#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->line = 1;
    state->c = 1;
    state->head = NULL;
    state->tail = NULL;
    state->cur_data = NULL;
}

void mt_token_state_free(mt_token_state* const state) {
    mt_token* t = state->head;
    while (t != NULL) {
        mt_token* cpy = t;
        t = t->next;
        if (cpy->type == mt_token(VAR)) {
            mt_buf_free(cpy->data.mt_var);
        }
        free(cpy);
    }
    if (state->cur_data != NULL) {
        mt_buf_free(state->cur_data);
    }
}

static void mt_token_add(mt_token_state* const state, mt_token_type type, mt_token_data data) {
    mt_token* token = malloc(sizeof(mt_token));
    token->line = state->line;
    token->c = state->c;
    token->type = type;
    token->data = data;
    token->next = NULL;
    if (state->head == NULL && state->tail == NULL) {
        state->head = token;
        state->tail = token;
    } else {
        state->tail->next = token;
        state->tail = token;
    }
}

static inline void mt_token_inc_char(mt_token_state* const state) {
    state->c++;
}

static inline void mt_token_inc_line(mt_token_state* const state) {
    state->c = 0;
    state->line++;
}

static inline void mt_token_add_no_data(mt_token_state* const state, mt_token_type type) {
    mt_token_add(state, type, (mt_token_data) { .mt_int = 0 });
}

static inline void mt_token_set_cur_data(mt_token_state* const state, mt_char c) {
    if (state->cur_data == NULL) {
        state->cur_data = mt_buf_init(MT_TOKEN_DATA_CHAR_BUF_SIZE);
    }
    mt_buf_push_char(state->cur_data, c);
}

static inline bool mt_token_is_skip(mt_char c) {
    return c.a != '\n' && c.a <= ' ';
}

static inline bool mt_token_is_var(mt_char c) {
    uint8_t conts = mt_char_cont(c.a);
    if (conts > 0 || (c.a >= 'a' && c.a <= 'z') || (c.a >= 'A' && c.a <= 'Z')) {
        return true;
    }
    return false;
}

static inline bool mt_token_is_num(mt_char c) {
    return c.a >= '0' && c.a <= '9';
}

static mt_var mt_token_state_nothing(mt_token_state* const state) {
    mt_char cur_char;
    bool has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    mt_token_inc_char(state);
    if (!has_chars || mt_token_is_skip(cur_char)) {
        return mt_var_bool(has_chars);
    }
    if (mt_token_is_num(cur_char)) {
        mt_token_set_cur_data(state, cur_char);
        state->state = mt_token_state(INT);
        return mt_var_bool(has_chars);
    }
    if (mt_token_is_var(cur_char)) {
        mt_token_set_cur_data(state, cur_char);
        state->state = mt_token_state(VAR);
        return mt_var_bool(has_chars);
    }
    switch (cur_char.a) {
        mt_token_quick_nothing(state, ASSIGN);
        mt_token_quick_nothing(state, PERIOD);
        mt_token_quick_nothing(state, L_BRACE);
        mt_token_quick_nothing(state, R_BRACE);
        mt_token_quick_nothing(state, COMMA);
        mt_token_quick_nothing(state, QUESTION);
        mt_token_quick_nothing(state, EQ);
        mt_token_quick_nothing(state, OR);
        mt_token_quick_nothing(state, L_BRACKET);
        mt_token_quick_nothing(state, R_BRACKET);
        mt_token_quick_nothing(state, DOLLAR);
        mt_token_quick_nothing(state, ADD);
        mt_token_quick_nothing(state, SUB);
        mt_token_quick_nothing(state, END);
        mt_token_quick_nothing(state, GREATER);
        case mt_token(LESS):
            has_chars = mt_buf_iter_peek(&state->iter, &cur_char);
            if (has_chars && cur_char.a == '>') {
                mt_token_add_no_data(state, mt_token(WRITE));
                has_chars = mt_buf_iter_next(&state->iter, &cur_char);
            } else {
                mt_token_add_no_data(state, mt_token(LESS));
            }
            break;
        case mt_token(SLASH):
            has_chars = mt_buf_iter_peek(&state->iter, &cur_char);
            if (has_chars && cur_char.a == '/') {
                state->state = mt_token_state(COMMENT);
            } else {
                mt_token_add_no_data(state, mt_token(SLASH));
            }
            break;
        case '\n':
            mt_token_add_no_data(state, mt_token(NL));
            mt_token_inc_line(state);
            break;
        default:
            break;
    }
    return mt_var_bool(has_chars);
}

static mt_var mt_token_state_comment(mt_token_state* const state) {
    mt_char cur_char;
    bool has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    if (!has_chars) {
        return mt_var_bool(true);
    }
    mt_token_inc_char(state);
    if (cur_char.a == '\n') {
        mt_token_inc_line(state);
        state->state = mt_token_state(NOTHING);
        return mt_var_bool(true);
    }
    return mt_var_bool(has_chars);
}

static mt_var mt_token_state_int(mt_token_state* const state) {
    mt_char cur_char, peek_char;
    bool has_chars = mt_buf_iter_peek(&state->iter, &peek_char);
    if (!(has_chars && ((peek_char.a >= '0' && peek_char.a <= '9') || peek_char.a == '_'))) {
        // @TODO check if valid var char
        uint8_t* endptr = &state->cur_data->data[state->cur_data->len];
        int64_t value = strtol((char*) state->cur_data->data, (char**) &endptr, 10);
        mt_buf_zero(state->cur_data);
        mt_token_add(state, mt_token(INT), (mt_token_data) { .mt_int = value });
        state->state = mt_token_state(NOTHING);
        return mt_var_bool(true);
    }
    has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    mt_token_inc_char(state);
    if (!has_chars) {
        return mt_var_bool(false);
    }
    if (mt_token_is_num(cur_char)) {
        if (!mt_buf_push_char(state->cur_data, cur_char)) {
            return mt_var_err(mt_err_token_buf_full());
        }
    } else if (cur_char.a != '_') {
        // @TODO invalid char
    }
    return mt_var_bool(has_chars);
}

static mt_var mt_token_state_var(mt_token_state* const state) {
    mt_char cur_char, peek_char;
    bool has_chars = mt_buf_iter_peek(&state->iter, &peek_char);
    if (!has_chars || !mt_token_is_var(peek_char)) {
        mt_token_add(state, mt_token(VAR), (mt_token_data) { .mt_var = state->cur_data });
        state->cur_data = NULL;
        state->state = mt_token_state(NOTHING);
        return mt_var_bool(true);
    }
    has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    mt_token_inc_char(state);
    if (!has_chars) {
        return mt_var_bool(false);
    }
    if (mt_token_is_var(cur_char)) {
        if (!mt_buf_push_char(state->cur_data, cur_char)) {
            return mt_var_err(mt_err_token_buf_full());
        }
    } else {
        // @TODO invalid char
    }
    return mt_var_bool(has_chars);
}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    mt_var has_chars = mt_var_bool(true);

    mt_buf_iter_init(buf, &state->iter);
    while (mt_var_is_bool(has_chars) && has_chars.data.mt_bool == true) {
        switch (state->state) {
            case mt_token_state(NOTHING):
                has_chars = mt_token_state_nothing(state);
                break;
            case mt_token_state(COMMENT):
                has_chars = mt_token_state_comment(state);
                break;
            case mt_token_state(VAR):
                has_chars = mt_token_state_var(state);
                break;
            case mt_token_state(INT):
                has_chars = mt_token_state_int(state);
                break;
        }
    }
    return has_chars;
}

static inline void mt_token_debug_print_space(size_t stop) {
    for (size_t i = 0; i < stop; i++) {
        putchar(' ');
    }
}

static inline void mt_token_debug_print_top(size_t ident_bracket, size_t ident_brace, mt_token* t) {
    putchar('\n');
    mt_token_debug_print_space(ident_bracket);
    mt_token_debug_print_space(ident_brace);
    printf("%lu:%lu:%c\n", t->line, t->c, t->type);
}

void mt_token_state_debug_print(const mt_token_state* const state) {
    size_t ident_brace = 0;
    size_t ident_bracket = 0;
    mt_token* t = state->head;
    while (t != NULL) {
        switch (t->type) {
            case mt_token(VAR):
                printf("%lu:%lu:", t->line, t->c);
                mt_buf_debug_print(t->data.mt_var);
                putchar(' ');
                break;
            case mt_token(INT):
                printf("%lu:%lu:%li ", t->line, t->c, t->data.mt_int);
                break;
            case mt_token(WRITE):
                printf("%lu:%lu:<> ", t->line, t->c);
                break;
            case mt_token(NL):
                printf("%lu:%lu:\\n\n", t->line, t->c);
                mt_token_debug_print_space(ident_bracket);
                mt_token_debug_print_space(ident_brace);
                break;
            case mt_token(L_BRACE):
                mt_token_debug_print_top(ident_bracket, ident_brace, t);
                ident_brace++;
                mt_token_debug_print_space(ident_bracket);
                mt_token_debug_print_space(ident_brace);
                break;
            case mt_token(R_BRACE):
                ident_brace--;
                mt_token_debug_print_top(ident_bracket, ident_brace, t);
                mt_token_debug_print_space(ident_bracket);
                mt_token_debug_print_space(ident_brace);
                break;
            case mt_token(L_BRACKET):
                mt_token_debug_print_top(ident_bracket, ident_brace, t);
                ident_bracket++;
                mt_token_debug_print_space(ident_bracket);
                mt_token_debug_print_space(ident_brace);
                break;
            case mt_token(R_BRACKET):
                ident_bracket--;
                mt_token_debug_print_top(ident_bracket, ident_brace, t);
                mt_token_debug_print_space(ident_bracket);
                mt_token_debug_print_space(ident_brace);
                break;
            default:
                printf("%lu:%lu:%c ", t->line, t->c, t->type);
                break;
        }
        t = t->next;
    }
}

int32_t mt_token_buf_info(mt_buf* const target, const mt_token* const token) {
    int32_t sub_total = 0;
    if (mt_buf_space_free(target) < 30) {
        return -1;
    }
    int32_t total = sprintf((char*) mt_buf_cur_raw(target), "Line: %lu, Char: %lu, ", token->line, token->c);
    if (total < 0) {
        return total;
    }
    target->len += total;
    switch (token->type) {
        case mt_token(VAR):
            if (token->data.mt_var->len > mt_buf_space_free(target)) {
                return total;
            }
            mt_buf_write(target, token->data.mt_var->data, token->data.mt_var->len);
            total += token->data.mt_var->len;
            break;
        case mt_token(INT):
            sub_total = sprintf((char*) mt_buf_cur_raw(target), "%li", token->data.mt_int);
            break;
        case mt_token(WRITE):
            mt_buf_write(target, "<>", 2);
            sub_total = 2;
            break;
        case mt_token(NL):
            mt_buf_write(target, "\\n", 2);
            sub_total = 2;
            break;
        default:
            mt_buf_write(target, &token->type, 1);
            sub_total = 1;
            break;
    }
    if (sub_total < 0) {
        return sub_total;
    }
    target->len += sub_total;
    total += sub_total;
    return total;
}
