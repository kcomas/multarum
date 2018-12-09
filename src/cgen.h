
#ifndef MT_CGEN_H
#define MT_CGEN_H

#include "mod.h"
#include "ast.h"

typedef struct {
    size_t if_size, if_len;
    uint8_t** if_pos; // jmps to fill at end of if
} mt_cgen_state;

#define mt_cgen_state_push_if(state, pos) state->if_pos[state->if_len++] = pos

#ifndef MT_CGEN_STATE_DEFAULT_IF_SIZE
#   define MT_CGEN_STATE_DEFAULT_IF_SIZE 30
#endif

void mt_cgen_state_init(mt_cgen_state* const state, size_t if_size);

void mt_cgen_state_free(mt_cgen_state* const state);

mt_var mt_cgen_build(mt_cgen_state* const state, const mt_ast* const ast, mt_mod* const mod, bool repl);

#endif
