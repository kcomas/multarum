
#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->head = NULL;
    state->tail = NULL;
}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    mt_char cur_char;
    bool has_chars = true;

    mt_buf_iter_init(buf, &state->iter);
    while (has_chars) {
        has_chars = mt_buf_iter_next(&state->iter, &cur_char);
        printf("%c\n", cur_char.a);
    }
    return mt_var_bool(true);
}
