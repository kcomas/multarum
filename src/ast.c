
#include "ast.h"

#define mt_ast_sym_table_init(tbl, target, size) \
    tbl->target.idx = 0; \
    tbl->target.hash = mt_hash_init(size)

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

static inline mt_ast_bop* mt_ast_create_bop(mt_ast* const left, mt_ast* const right) {
    mt_ast_bop* bop = (mt_ast_bop*) malloc(sizeof(mt_ast_bop));
    bop->left = left;
    bop->right = right;
    return bop;
}

static mt_ast* mt_ast_fn_init(void) {
    mt_ast_fn* fn = (mt_ast_fn*) malloc(sizeof(mt_ast_fn));
    fn->sym_table = (mt_ast_sym_table*) malloc(sizeof(mt_ast_sym_table));
    mt_ast_fn_access(fn, arg_table).hash = NULL;
    mt_ast_fn_access(fn, local_table).hash = NULL;
    fn->ops_head = mt_ast_add_op_list();
    fn->ops_tail = fn->ops_head;
    return mt_ast_node(FN, fn, fn);
}

static void mt_ast_free_walk(mt_ast* const ast);

static inline void mt_ast_free_bop(mt_ast* const ast) {
    mt_ast_free_walk(ast->node.bop->left);
    mt_ast_free_walk(ast->node.bop->right);
    free(ast->node.bop);
}

#define mt_ast_free_bop_case(op, ast) \
    case mt_ast(op): \
        mt_ast_free_bop(ast); \
        break

#define mt_ast_free_sym_table(ast, tgt) \
    if (ast->node.fn->sym_table->tgt.hash != NULL) { \
        mt_hash_free(ast->node.fn->sym_table->tgt.hash); \
    }

#define mt_ast_free_list(ast, tgt) \
    ops = tgt; \
    while (ops != NULL) { \
        mt_ast_free_walk(ops->op); \
        tmp = ops; \
        ops = ops->next; \
        free(tmp); \
    }

static void mt_ast_free_walk(mt_ast* const ast) {
    if (ast == NULL) {
        return;
    }
    mt_ast_op_list* ops;
    mt_ast_op_list* tmp;
    mt_ast_if_cond* conds;
    mt_ast_if_cond* conds_tmp;
    switch (ast->type) {
        case mt_ast(NULL):
            break;
        case mt_ast(FN):
            mt_ast_free_list(ast, ast->node.fn->ops_head);
            mt_ast_free_sym_table(ast, arg_table);
            mt_ast_free_sym_table(ast, local_table);
            free(ast->node.fn->sym_table);
            free(ast->node.fn);
            break;
        case mt_ast(VAR):
        case mt_ast(ARG):
        case mt_ast(INT):
            break;
        case mt_ast(STR):
            mt_buf_free(ast->node.value.mt_str);
            break;
        mt_ast_free_bop_case(ASSIGN, ast);
        mt_ast_free_bop_case(EQ, ast);
        case mt_ast(IF):
            conds = ast->node.if_smt->head;
            while (conds != NULL) {
                if (conds->cond != NULL) {
                    mt_ast_free_walk(conds->cond);
                }
                mt_ast_free_list(ast, conds->body_head);
                conds_tmp = conds;
                conds = conds->next;
                free(conds_tmp);
            }
            free(ast->node.if_smt);
            break;
        mt_ast_free_bop_case(OR, ast);
        case mt_ast(CALL):
            mt_ast_free_list(ast, ast->node.call->args_head);
            free(ast->node.call);
            break;
        mt_ast_free_bop_case(MUL, ast);
        mt_ast_free_bop_case(ADD, ast);
        mt_ast_free_bop_case(SUB, ast);
        mt_ast_free_bop_case(WRITE, ast);
    }
    free(ast);
}

void mt_ast_free(mt_ast_state* const state) {
    mt_ast_free_walk(state->ast);
}

#define mt_ast_fn_add_table(tbl, buf, target, size) \
    if (tbl->target.hash == NULL) { \
        mt_ast_sym_table_init(tbl, target, size); \
    } \
    if (mt_var_is_null(mt_hash_get(tbl->target.hash, buf))) { \
        mt_hash_insert(tbl->target.hash, buf, mt_var_int(tbl->target.idx++)); \
    }

#define mt_ast_symbol_in(where, tbl, buf) \
    (tbl->where.hash != NULL && !mt_var_is_null(mt_hash_get(tbl->where.hash, buf)))

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

#define mt_ast_inc_token(state) state->cur_token = mt_ast_peek_token(state);

#define mt_ast_inc_token_sub(state) state.cur_token = state.cur_token->next;

#define mt_ast_inc_token2(state) \
    mt_ast_inc_token(state); \
    if (state->cur_token == NULL) { \
        return mt_var_err(mt_err_ast_token_end()); \
    } \
    mt_ast_inc_token(state);

static inline void mt_ast_flush_nl(mt_ast_state* const state) {
    while (state->cur_token != NULL && state->cur_token->type == mt_token(NL)) {
        mt_ast_inc_token(state);
    }
}

static inline mt_ast_if_cond* mt_ast_if_cond_init() {
    mt_ast_if_cond* cond = malloc(sizeof(mt_ast_if_cond));
    cond->cond = NULL;
    cond->body_head = mt_ast_add_op_list();
    cond->body_tail = cond->body_head;
    cond->next = NULL;
    return cond;
}

static mt_var mt_ast_next_token(mt_ast_state* const state, mt_ast** const cur_tree);

static inline mt_ast_sym_table* mt_ast_get_sym(mt_ast* const ast) {
    switch (ast->type) {
        case mt_ast(FN):
            return ast->node.fn->sym_table;
        case mt_ast(IF):
            return ast->node.if_smt->sym_table;
        case mt_ast(CALL):
            return ast->node.call->sym_table;
        default:
            return NULL;
    }
}

static mt_var mt_ast_build_if(mt_ast_state* const state, mt_ast** const cur_tree) {
    mt_ast_if* if_smt = malloc(sizeof(mt_ast_if));
    if_smt->sym_table = mt_ast_get_sym(state->ast);
    if_smt->head = mt_ast_if_cond_init();
    if_smt->tail = if_smt->head;
    mt_ast_state sub_state;
    sub_state.mode = mt_ast_state(IF_COND);
    sub_state.cur_token = state->cur_token;
    sub_state.ast = mt_ast_node(IF, if_smt, if_smt);
    while (sub_state.cur_token != NULL) {
        mt_ast* sub_tree = NULL;
        mt_var rst = mt_ast_next_token(&sub_state, &sub_tree);
        if (mt_var_is_err(rst)) {
            return rst;
        }
        switch (sub_state.mode) {
            case mt_ast_state(IF_COND):
                if_smt->tail->cond = sub_tree;
                sub_state.mode = mt_ast_state(IF_BODY);
                break;
            case mt_ast_state(IF_BODY):
                if_smt->tail->body_tail->op = sub_tree;
                if (sub_state.cur_token != NULL && sub_state.cur_token->type == mt_token(R_BRACKET)) {
                    mt_ast_inc_token_sub(sub_state);
                    while (sub_state.cur_token->type == mt_token(NL)) {
                        mt_ast_inc_token_sub(sub_state);
                    }
                    if (sub_state.cur_token != NULL && sub_state.cur_token->type == mt_token(R_BRACKET)) {
                        mt_ast_inc_token_sub(sub_state);
                        state->cur_token = sub_state.cur_token;
                        *cur_tree = sub_state.ast;
                        return mt_var_bool(true);
                    }
                    sub_state.mode = mt_ast_state(IF_COND);
                    if_smt->tail->next = mt_ast_if_cond_init();
                    if_smt->tail = if_smt->tail->next;
                }
                if_smt->tail->body_tail->next = mt_ast_add_op_list();
                if_smt->tail->body_tail = if_smt->tail->body_tail->next;
                break;
            default:
                return mt_var_err(mt_err_ast_invalid_if_state());
        }
    }
    return mt_var_err(mt_err_ast_invalid_if_state());
}

static mt_var mt_ast_build_call(mt_ast_state* const state, mt_ast** cur_tree, mt_buf* const target) {
    mt_ast_call* call = (mt_ast_call*) malloc(sizeof(mt_ast_call));
    call->arg_count = 0;
    call->sym_table = mt_ast_get_sym(state->ast);
    call->target = target;
    call->args_head = mt_ast_add_op_list();
    call->args_tail = call->args_head;
    mt_ast_state sub_state;
    sub_state.mode = mt_ast_state(CALL);
    sub_state.cur_token = state->cur_token;
    sub_state.ast = mt_ast_node(CALL, call, call);
    while (sub_state.cur_token != NULL) {
        mt_ast* sub_tree = NULL;
        mt_var rst = mt_ast_next_token(&sub_state, &sub_tree);
        if (mt_var_is_err(rst)) {
            return rst;
        }
        call->args_tail->op = sub_tree;
        call->arg_count++;
        if (sub_state.cur_token->type == mt_token(R_BRACE)) {
            state->cur_token = sub_state.cur_token;
            mt_ast_inc_token(state);
            *cur_tree = sub_state.ast;
            return mt_ast_next_token(state, cur_tree);
        }
        call->args_tail->next = mt_ast_add_op_list();
        call->args_tail = call->args_tail->next;
    }
    return mt_var_err(mt_err_ast_invalid_call_state());
}

#define mt_ast_quic_bop(TYPE) \
    *cur_tree = mt_ast_node(TYPE, bop, mt_ast_create_bop(*cur_tree, NULL)); \
    mt_ast_inc_token(state); \
    return mt_ast_next_token(state, &(*cur_tree)->node.bop->right); \

#define mt_ast_invalid_state(TYPE) \
    if (state->mode == mt_ast_state(TYPE)) { \
        return mt_ast_token_invalid(state->cur_token); \
    }

static mt_var mt_ast_next_token(mt_ast_state* const state, mt_ast** const cur_tree) {
    mt_ast_state sub_state;
    mt_ast* sub_tree = NULL;
    mt_var build_rst;
    if (state->cur_token == NULL) {
        return mt_var_bool(true);
    }
    switch (state->cur_token->type) {
        case mt_token(VAR):
            switch (state->mode) {
                case mt_ast_state(MAIN):
                case mt_ast_state(FN):
                case mt_ast_state(IF_COND):
                case mt_ast_state(IF_BODY):
                case mt_ast_state(CALL):
                    if (mt_ast_peek_token_is_type(state, L_BRACE)) {
                        if (!mt_ast_symbol_in(arg_table, mt_ast_get_sym(state->ast), state->cur_token->data.mt_var) && !mt_ast_symbol_in(local_table, mt_ast_get_sym(state->ast), state->cur_token->data.mt_var)) {
                            return mt_var_err(mt_err_ast_undef());
                        }
                        mt_buf* target = state->cur_token->data.mt_var;
                        mt_ast_inc_token2(state);
                        return mt_ast_build_call(state, cur_tree, target);
                    }
                    if (mt_ast_symbol_in(arg_table, mt_ast_get_sym(state->ast), state->cur_token->data.mt_var)) {
                        *cur_tree = mt_ast_node(ARG, value, mt_ast_value(mt_var, state->cur_token->data.mt_var));
                    } else {
                        *cur_tree = mt_ast_node(VAR, value, mt_ast_value(mt_var, state->cur_token->data.mt_var));
                        mt_ast_fn_add_table(mt_ast_get_sym(state->ast), state->cur_token->data.mt_var, local_table, MT_AST_LOCAL_SIZE);
                    }
                    break;
                case mt_ast_state(ARGS):
                    if (mt_ast_symbol_in(arg_table, mt_ast_get_sym(state->ast), state->cur_token->data.mt_var)) {
                        return mt_var_err(mt_err_ast_dup_arg());
                    }
                    mt_ast_fn_add_table(mt_ast_get_sym(state->ast), state->cur_token->data.mt_var, arg_table, MT_AST_LOCAL_SIZE);
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
        case mt_token(STR):
            *cur_tree = mt_ast_node(STR, value, mt_ast_value(mt_str, state->cur_token->data.mt_str));
            mt_ast_inc_token(state);
            return mt_ast_next_token(state, cur_tree);
        case mt_token(ASSIGN):
            mt_ast_invalid_state(IF_COND);
            mt_ast_quic_bop(ASSIGN);
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
            state->cur_token = sub_state.cur_token;
            *cur_tree = sub_state.ast;
            return mt_var_bool(true);
        case mt_token(L_BRACE):
            mt_ast_inc_token(state);
            sub_tree = NULL;
            build_rst = mt_ast_next_token(state, &sub_tree);
            if (mt_var_is_err(build_rst)) {
                return build_rst;
            }
            *cur_tree = sub_tree;
            return mt_ast_next_token(state, cur_tree);
        case mt_token(R_BRACE):
            switch (state->mode) {
                case mt_ast_state(ARGS):
                    if (mt_ast_peek_token_is_type(state, L_BRACKET)) {
                        mt_ast_inc_token2(state);
                        state->mode = mt_ast_state(FN);
                        return mt_ast_next_token(state, cur_tree);
                    }
                case mt_ast_state(CALL):
                    return mt_var_bool(true);
                default:
                    mt_ast_inc_token(state);
                    return mt_var_bool(true);
                    break;
            }
        case mt_token(COMMA):
            if (state->mode == mt_ast_state(CALL)) {
                mt_ast_inc_token(state);
                return mt_var_bool(true);
            }
            return mt_ast_token_invalid(state->cur_token);
        case mt_token(QUESTION):
            if (!mt_ast_peek_token_is_type(state, L_BRACKET)) {
                return mt_ast_token_invalid(state->cur_token);
            }
            mt_ast_inc_token2(state);
            return mt_ast_build_if(state, cur_tree);
        case mt_token(EQ):
            mt_ast_quic_bop(EQ);
        case mt_token(OR):
            mt_ast_quic_bop(OR);
        case mt_token(L_BRACKET):
            if (state->mode == mt_ast_state(IF_COND)) {
                mt_ast_inc_token(state);
                return mt_var_bool(true);
            }
            return mt_ast_token_invalid(state->cur_token);
        case mt_token(R_BRACKET):
            switch (state->mode) {
                case mt_ast_state(FN):
                    mt_ast_inc_token(state);
                case mt_ast_state(IF_BODY):
                    return mt_var_bool(false);
                default:
                    return mt_ast_token_invalid(state->cur_token);
            }
        case mt_token(DOLLAR):
            mt_ast_invalid_state(IF_COND);
            if (!mt_ast_peek_token_is_type(state, L_BRACE)) {
                return mt_ast_token_invalid(state->cur_token);
            }
            mt_ast_inc_token2(state);
            return mt_ast_build_call(state, cur_tree, NULL);
        case mt_token(MUL):
            mt_ast_invalid_state(IF_COND);
            mt_ast_quic_bop(MUL);
        case mt_token(ADD):
            mt_ast_invalid_state(IF_COND);
            mt_ast_quic_bop(ADD);
        case mt_token(SUB):
            mt_ast_invalid_state(IF_COND);
            mt_ast_quic_bop(SUB);
        case mt_token(WRITE):
            mt_ast_invalid_state(IF_COND);
            mt_ast_quic_bop(WRITE);
        case mt_token(NL):
            mt_ast_flush_nl(state);
            if (*cur_tree == NULL && state->cur_token != NULL) {
                return mt_ast_next_token(state, cur_tree);
            }
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
        state->ast->node.fn->ops_tail->op = cur_tree;
        if (mt_var_is_bool(rst) && mt_var_bool_value(rst) == false) {
            break;
        }
        state->ast->node.fn->ops_tail->next = mt_ast_add_op_list();
        state->ast->node.fn->ops_tail = state->ast->node.fn->ops_tail->next;
    }
    return mt_var_bool(true);
}

static inline void mt_ast_print_spaces(uint32_t indent) {
    for (uint32_t i = 0; i < indent; i++) {
        putchar(' ');
    }
}

#define mt_ast_print_bop(CASE, c) \
    case mt_ast(CASE): \
        printf("%c\n", c); \
        mt_ast_debug_print(ast->node.bop->left, indent + 1); \
        mt_ast_debug_print(ast->node.bop->right, indent + 1); \
        break

#define mt_ast_debug_ops_list(ast, indent, tgt) \
    list = tgt; \
    while (list != NULL) { \
        mt_ast_debug_print(list->op, indent + 1); \
        list = list->next; \
    }

void mt_ast_debug_print(const mt_ast* const ast, uint32_t indent) {
    if (ast == NULL) {
       return;
    }
    mt_ast_print_spaces(indent);
    mt_ast_op_list* list;
    mt_ast_if_cond* ifs;
    switch (ast->type) {
        case mt_ast(NULL):
            printf("NULL\n");
            break;
        case mt_ast(FN):
            printf("FN\n");
            list = ast->node.fn->ops_head;
            while (list != NULL) {
                mt_ast_debug_print(list->op, indent + 1);
                list = list->next;
            }
            break;
        mt_ast_print_bop(ASSIGN, ':');
        case mt_ast(VAR):
        case mt_ast(ARG):
            mt_buf_debug_print(ast->node.value.mt_var);
            printf("\n");
            break;
        case mt_ast(INT):
            printf("%lu\n", ast->node.value.mt_int);
            break;
        case mt_ast(STR):
            putchar('"');
            mt_buf_debug_print(ast->node.value.mt_str);
            printf("\"\n");
            break;
        case mt_ast(IF):
            printf("IF\n");
            ifs = ast->node.if_smt->head;
            while (ifs != NULL) {
                mt_ast_print_spaces(indent);
                printf("COND\n");
                mt_ast_debug_print(ifs->cond, indent + 1);
                mt_ast_print_spaces(indent);
                printf("BODY\n");
                mt_ast_debug_ops_list(ast, indent, ifs->body_head);
                ifs = ifs->next;
            }
            break;
        mt_ast_print_bop(EQ, '=');
        mt_ast_print_bop(OR, '|');
        case mt_ast(CALL):
            printf("CALL %d ", ast->node.call->arg_count);
            if (ast->node.call->target == NULL) {
                printf("SELF");
            } else {
                mt_buf_debug_print(ast->node.call->target);
            }
            printf("\n");
                mt_ast_debug_ops_list(ast, indent, ast->node.call->args_head);
            break;
        mt_ast_print_bop(MUL, '*');
        mt_ast_print_bop(ADD, '+');
        mt_ast_print_bop(SUB, '-');
        case mt_ast(WRITE):
            printf("<>\n");
            mt_ast_debug_print(ast->node.bop->left, indent + 1);
            mt_ast_debug_print(ast->node.bop->right, indent + 1);
            break;
    }
}
