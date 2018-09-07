#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->line = 0;
    state->c = 0;
    state->head = NULL;
    state->tail = NULL;
}

void mt_token_state_free(mt_token_state* const state) {
    mt_token* t = state->head;
    while (t != NULL) {
        mt_token* cpy = t;
        t = t->next;
        free(cpy);
    }
}

static void mt_add_token(mt_token_state* const state, mt_token_type type, mt_token_data data) {
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

static void mt_token_inc_char(mt_token_state* const state) {
    state->c++;
}

static void mt_token_inc_line(mt_token_state* const state) {
    state->c = 0;
    state->line++;
}

static void mt_token_add_no_data(mt_token_state* const state, mt_token_type type) {
    mt_add_token(state, type, (mt_token_data) { .mt_int = 0 });
}

#define mt_token_quick_nothing(state, token) case mt_token(token): mt_token_add_no_data(state, mt_token(token)); break

static mt_var mt_token_state_nothing(mt_token_state* const state) {
    mt_char cur_char;
    bool has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    mt_token_inc_char(state);
    if (!has_chars) {
        return mt_var_bool(has_chars);
    }
    switch (cur_char.a) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        mt_token_quick_nothing(state, ASSIGN);
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
        case mt_token(GREATER):
            has_chars = mt_buf_iter_peek(&state->iter, &cur_char);
            if (has_chars && cur_char.a == '>') {
                mt_token_add_no_data(state, mt_token(WRITE));
                has_chars = mt_buf_iter_next(&state->iter, &cur_char);
            } else {
                mt_token_add_no_data(state, mt_token(GREATER));
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
        case ' ':
        case '\t':
        case '\0':
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
        }
    }
    return has_chars;
}

static void mt_token_debug_print_space(size_t stop) {
    for (size_t i = 0; i < stop; i++) {
        putchar(' ');
    }
}

static void mt_token_debug_print_top(size_t ident_bracket, size_t ident_brace, mt_token* t) {
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
            case mt_token(WRITE):
                printf("%lu:%lu:>> ", t->line, t->c);
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