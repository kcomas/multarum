
#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
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

static void mt_token_state_nothing(mt_token_state* const state, mt_char cur_char) {

}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    mt_char cur_char;
    bool has_chars = true;

    mt_buf_iter_init(buf, &state->iter);
    while (has_chars) {
        has_chars = mt_buf_iter_next(&state->iter, &cur_char);
        // printf("%c\n", cur_char.a);
        switch (state->state) {
            case mt_token_state(NOTHING):
                mt_token_state_nothing(state, cur_char);
                break;
        }
    }
    return mt_var_bool(true);
}
