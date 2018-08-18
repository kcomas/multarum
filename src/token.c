
#include "token.h"

void mt_token_state_init(mt_token_state* const state) {
    state->state = mt_token_state(NOTHING);
    state->buf_pos = 0;
    state->name = mt_buf_init(MT_MAX_BUF_NAME);
    state->head = NULL;
    state->tail = NULL;
}

mt_var mt_tokenize_buf(mt_token_state* const state, const mt_buf* const buf) {
    /**
    while (state->buf_pos < buf->len) {

    }
    */
    return mt_var_bool(true);
}
