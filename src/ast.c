
#include "ast.h"

#define mt_ast_sym_table_init(fn, target, size) \
    fn->sym_table->target.idx = 0; \
    fn->sym_table->target.hash = mt_hash_init(size)

static inline mt_ast_op_list* mt_ast_add_op_list(void) {
    mt_ast_op_list* ops = (mt_ast_op_list*) malloc(sizeof(mt_ast_op_list));
    ops->op = NULL;
    ops->next = NULL;
    return ops;
}

static inline mt_ast* mt_ast_create_node(mt_ast_type type, mt_ast_node node) {
    mt_ast* ast = (mt_ast*) malloc(sizeof(mt_ast));
    ast->type = type;
    ast->node = node;
    return ast;
}

#define mt_ast_value(name, value) (mt_ast_value) { .name = value }

#define mt_ast_node(type, name, _node) mt_ast_create_node(mt_ast(type), (mt_ast_node) { .name = _node })

static mt_ast* mt_ast_fn_init(void) {
    mt_ast_fn* fn = malloc(sizeof(mt_ast_fn));
    fn->sym_table.args_table.hash = NULL;
    fn->sym_table.local_table.hash = NULL;
    fn->sym_table.fn_table.hash = NULL;
    fn->ops = mt_ast_add_op_list();
    return mt_ast_node(FN, fn, fn);
}

void mt_ast_init(mt_ast_state* const state) {
    state->mode = mt_ast_state(MAIN);
    state->cur_token = NULL;
    state->ast = mt_ast_fn_init();
}

static void mt_ast_add_data_to_tree(mt_ast* cur_tree, mt_ast* const new_node) {
    switch (cur_tree->type) {
        case mt_ast(NULL):
            cur_tree = new_node;
            break;
        default:
            break;
    }
}

static mt_var mt_ast_next_token(mt_ast_state* const state, mt_ast* cur_tree) {
    switch (state->cur_token->type) {
        case mt_token(VAR):
            switch (state->mode) {
                case mt_ast_state(MAIN):
                case mt_ast_state(FN):
                    break;
                case mt_ast_state(ARGS):
                    break;
            }
        case mt_token(INT):
            mt_ast_add_data_to_tree(cur_tree, mt_ast_node(INT, value, mt_ast_value(mt_int, state->cur_token->data.mt_int)));
        default:
            return mt_var_err(mt_err_ast_build_fail("Invalid Token Found"));
    }
    return mt_ast_next_token(state, cur_tree);
}

mt_var mt_ast_build(mt_ast_state* const state, mt_token* const tokens) {
    state->cur_token = tokens;
    while (state->cur_token != NULL) {
        mt_ast cur_tree = mt_ast_null();
        mt_var rst = mt_ast_next_token(state, &cur_tree);
        if (mt_var_is_err(rst)) {
            return rst;
        }
    }
    return mt_var_bool(true);
}
