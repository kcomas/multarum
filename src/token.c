
#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->head = NULL;
    state->tail = NULL;
}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    mt_buf_iter char_iter;
    mt_char cur_char;

    mt_buf_iter_init(buf, &state->iter);
    /**
    while (state->iter->buf_pos < buf->len) {

    }
    */
    return mt_var_bool(true);
}
