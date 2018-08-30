#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->line = 0;
    state->c = 0;
    state->head = NULL;
    state->tail = NULL;
}

static void mt_add_token(mt_token_state* const state, mt_token_type type, mt_token_data data) {
    mt_token* token = malloc(sizeof(mt_token));
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

static void mt_add_token_no_data(mt_token_state* const state, mt_token_type type) {
    mt_add_token(state, type, (mt_token_data) { .mt_int = 0 });
}

#define mt_token_quick_nothing(c, state, token) case c: mt_add_token_no_data(state, mt_token(token)); break

static mt_var mt_token_state_nothing(mt_token_state* const state) {
    mt_char cur_char;
    bool has_chars = mt_buf_iter_next(&state->iter, &cur_char);
    switch (cur_char.a) {
        mt_token_quick_nothing(':', state, ASSIGN);
        mt_token_quick_nothing('(', state, L_BRACE);
        mt_token_quick_nothing(')', state, R_BRACE);
        mt_token_quick_nothing(',', state, COMMA);
        mt_token_quick_nothing('?', state, QUESTION);
        default:
            break;
    }
    return mt_var_bool(has_chars);
}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    mt_var has_chars = mt_var_bool(true);

    mt_buf_iter_init(buf, &state->iter);
    while (mt_var_is_bool(has_chars) && has_chars.data.mt_bool == true) {
        // printf("%c\n", cur_char.a);
        switch (state->state) {
            case mt_token_state(NOTHING):
                has_chars = mt_token_state_nothing(state);
                break;
        }
    }
    return has_chars;
}
