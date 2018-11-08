
#include "ast.h"

#define mt_ast_sym_table_init(fn, target, size) \
    fn->sym_table.target.idx = 0; \
    fn->sym_table.target.hash = mt_hash_init(size)

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

#define mt_ast_fn_access(fn, target) fn->sym_table.target

static inline mt_ast_bop* mt_ast_create_bop(mt_ast* const left, mt_ast* const right) {
    mt_ast_bop* bop = (mt_ast_bop*) malloc(sizeof(mt_ast_bop));
    bop->left = left;
    bop->right = right;
    return bop;
}

static mt_ast* mt_ast_fn_init(void) {
    mt_ast_fn* fn = malloc(sizeof(mt_ast_fn));
    mt_ast_fn_access(fn, arg_table).hash = NULL;
    mt_ast_fn_access(fn, local_table).hash = NULL;
    mt_ast_fn_access(fn, fn_table).hash = NULL;
    fn->ops_head = mt_ast_add_op_list();
    fn->ops_tail = fn->ops_head;
    return mt_ast_node(FN, fn, fn);
}

#define mt_ast_fn_add_table(fn, buf, target, size) \
    if (mt_ast_fn_access(fn, target).hash == NULL) { \
        mt_ast_sym_table_init(fn, target, size); \
    } \
    if (mt_var_is_null(mt_hash_get(mt_ast_fn_access(fn, target).hash, buf))) { \
        mt_hash_insert(mt_ast_fn_access(fn, target).hash, buf, mt_var_int(mt_ast_fn_access(fn, target).idx++)); \
    }

#define mt_ast_symbol_in_args(fn, buf) \
    (mt_ast_fn_access(fn, arg_table).hash != NULL && mt_var_is_null(mt_hash_get(mt_ast_fn_access(fn, arg_table).hash, buf)))

void mt_ast_init(mt_ast_state* const state) {
    state->mode = mt_ast_state(MAIN);
    state->cur_token = NULL;
    state->ast = mt_ast_fn_init();
}

static mt_var mt_ast_token_invalid(const mt_token* const cur_token) {
    mt_buf* err_buf;
    static const char* err_msg = "Invalid Token Found, ";
    err_buf = mt_buf_init(200);
    mt_buf_write(err_buf, err_msg, 21);
    mt_token_buf_info(err_buf, cur_token);
    return mt_var_err(mt_err_ast_build_fail(err_buf));
}

#define mt_ast_peek_token(state) state->cur_token->next

#define mt_ast_peek_token_is_type(state, TYPE) \
    (state->cur_token->next != NULL && state->cur_token->next->type == mt_token(TYPE))

#define mt_ast_inc_token(state) do { \
        if (mt_ast_peek_token(state) == NULL) { \
            return mt_var_err(mt_err_ast_token_end()); \
        } \
        state->cur_token = mt_ast_peek_token(state); \
    } while (0)

#define mt_ast_inc_token2(state) \
    mt_ast_inc_token(state); \
    mt_ast_inc_token(state);

static mt_var mt_ast_next_token(mt_ast_state* const state, mt_ast** const cur_tree) {
    mt_ast_state sub_state;
    mt_var build_rst;
    if (state->cur_token == NULL) {
        return mt_var_bool(true);
    }
    switch (state->cur_token->type) {
        case mt_token(VAR):
            switch (state->mode) {
                case mt_ast_state(MAIN):
                case mt_ast_state(FN):
                case mt_ast_state(IF):
                    if (mt_ast_symbol_in_args(state->ast->node.fn, state->cur_token->data.mt_var)) {
                        *cur_tree = mt_ast_node(ARG, value, mt_ast_value(mt_var, state->cur_token->data.mt_var));
                    } else {
                        *cur_tree = mt_ast_node(VAR, value, mt_ast_value(mt_var, state->cur_token->data.mt_var));
                        mt_ast_fn_add_table(state->ast->node.fn, state->cur_token->data.mt_var, local_table, MT_AST_LOCAL_SIZE);
                    }
                    break;
                case mt_ast_state(ARGS):
                    if (mt_ast_symbol_in_args(state->ast->node.fn, state->cur_token->data.mt_var)) {
                        return mt_var_err(mt_err_ast_dup_arg());
                    }
                    mt_ast_fn_add_table(state->ast->node.fn, state->cur_token->data.mt_var, arg_table, MT_AST_LOCAL_SIZE);
                    if (mt_ast_peek_token_is_type(state, R_BRACE)) {
                        mt_ast_inc_token(state);
                        if (!mt_ast_peek_token_is_type(state, L_BRACKET)) {
                            return mt_ast_token_invalid(state->cur_token);
                        }
                        mt_ast_inc_token(state);
                        state->mode = mt_ast_state(FN);
                    } else if (!mt_ast_peek_token_is_type(state, VAR)) {
                        return mt_ast_token_invalid(state->cur_token);
                    }
                    break;
            }
            mt_ast_inc_token(state);
            return mt_ast_next_token(state, cur_tree);
        case mt_token(INT):
            *cur_tree = mt_ast_node(INT, value, mt_ast_value(mt_int, state->cur_token->data.mt_int));
            mt_ast_inc_token(state);
            return mt_ast_next_token(state, cur_tree);
        case mt_token(ASSIGN):
            *cur_tree = mt_ast_node(ASSIGN, bop, mt_ast_create_bop(*cur_tree, NULL));
            mt_ast_inc_token(state);
            return mt_ast_next_token(state, &(*cur_tree)->node.bop->right);
        case mt_token(PERIOD):
            if (!mt_ast_peek_token_is_type(state, L_BRACE)) {
                return mt_ast_token_invalid(state->cur_token);
            }
            mt_ast_inc_token2(state); // in fn def
            mt_ast_init(&sub_state);
            sub_state.mode = mt_ast_state(ARGS);
            build_rst = mt_ast_build(&sub_state, state->cur_token);
            if (mt_var_is_err(build_rst)) {
                return build_rst;
            }
            *cur_tree = sub_state.ast;
            return mt_var_bool(true);
        case mt_token(QUESTION):
            if (!mt_ast_peek_token_is_type(state, L_BRACKET)) {
                return mt_ast_token_invalid(state->cur_token);
            }
            mt_ast_inc_token2(state);
            // @TODO parse if
            exit(1);
        case mt_token(NL):
            mt_ast_inc_token(state);
            return mt_var_bool(true);
        default:
            return mt_ast_token_invalid(state->cur_token);
    }
}

mt_var mt_ast_build(mt_ast_state* const state, mt_token* const tokens) {
    state->cur_token = tokens;
    while (state->cur_token != NULL) {
        mt_ast* cur_tree = NULL;
        mt_var rst = mt_ast_next_token(state, &cur_tree);
        if (mt_var_is_err(rst)) {
            return rst;
        }
        if (cur_tree != NULL) {
            state->ast->node.fn->ops_tail->op = cur_tree;
            state->ast->node.fn->ops_tail->next = mt_ast_add_op_list();
            state->ast->node.fn->ops_tail = state->ast->node.fn->ops_tail->next;
            mt_ast_debug_print(state->ast, 0);
        }
    }
    return mt_var_bool(true);
}

void mt_ast_debug_print(const mt_ast* const ast, uint32_t indent) {
    if (ast == NULL) {
       return;
    }
    for (uint32_t i = 0; i < indent; i++) {
        putchar(' ');
    }
    switch (ast->type) {
        case mt_ast(NULL):
            printf("NULL\n");
            break;
        case mt_ast(FN):
            printf("FN\n");
            mt_ast_op_list* list = ast->node.fn->ops_head;
            while (list != NULL) {
                mt_ast_debug_print(list->op, indent + 1);
                list = list->next;
            }
            break;
        case mt_ast(ASSIGN):
            printf(":\n");
            mt_ast_debug_print(ast->node.bop->left, indent + 1);
            mt_ast_debug_print(ast->node.bop->right, indent + 1);
            break;
        case mt_ast(VAR):
        case mt_ast(ARG):
            mt_buf_debug_print(ast->node.value.mt_var);
            printf("\n");
            break;
        case mt_ast(INT):
            printf("%lu\n", ast->node.value.mt_int);
            break;
    }
}
