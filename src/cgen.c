
#include "cgen.h"


void mt_cgen_state_init(mt_cgen_state* const state, size_t if_size) {
    state->if_size = if_size;
    state->if_len = 0;
    state->if_pos = (uint8_t**) malloc(sizeof(uint8_t*) * if_size);
}

void mt_cgen_state_free(mt_cgen_state* const state) {
    free(state->if_pos);
}

#define mt_cgen_ck_err(rst) \
    if (mt_var_is_err(rst)) { \
        return rst; \
    }

#define mt_cgen_walk_bop_side(state, ast, mod, tbl, side) \
    rst = mt_cgen_walk(state, ast->node.bop->side, mod, tbl); \
    mt_cgen_ck_err(rst)

#define mt_cgen_basic_bop(state, ast, mod, tbl, op) \
    mt_cgen_walk_bop_side(state, ast, mod, tbl, left); \
    mt_cgen_walk_bop_side(state, ast, mod, tbl, right); \
    mt_write_byte(mod, op)

#define mt_cgen_basic_bop_case(state, ast, mod, tbl, op) \
    case mt_ast(op): \
        mt_cgen_basic_bop(state, ast, mod, tbl, mt_pfx(op)); \
        break

static inline uint8_t* mt_cgen_walk_jmp_sva(mt_mod* const mod, mt_op op) {
    uint32_t jmp_v = 0;
    mt_write_jmp(mod, op, &jmp_v);
    return &mod->bytes[mod->len - 4];
}

static inline void mt_cgen_walk_jmp_svb(mt_mod* const mod, uint8_t* jmp_hdl) {
    if (jmp_hdl != NULL) {
        uint32_t jmp_v = (uint32_t) mod->len;
        memcpy(jmp_hdl, &jmp_v, sizeof(uint32_t));
    }
}

#define mt_cgen_ops_list_walk(state, ast, mod, target, head) \
    ops = ast->node.target->head; \
    while (ops != NULL) { \
        rst = mt_cgen_walk(state, ops->op, mod, ast->node.target->sym_table); \
        mt_cgen_ck_err(rst); \
        ops = ops->next; \
    }

static inline mt_var mt_cgen_ld_arg(const mt_buf* const buf, mt_mod* const mod, const mt_ast_sym_table* const tbl) {
    mt_var rst = mt_hash_get(tbl->arg_table->hash, buf);
    if (mt_var_is_null(rst)) {
        return rst;
    }
    mt_write_byte(mod, mt_pfx(LD_ARG));
    mt_write_byte(mod, (uint8_t) rst.data.mt_int);
    return mt_var_bool(true);
}

#define mt_cgen_arg_exists(tgt, mod, tbl) \
    (tbl->arg_table->hash != NULL && !mt_var_is_null(mt_cgen_ld_arg(tgt, mod, tbl)))

static inline mt_var mt_cgen_ld_var(const mt_buf* const buf, mt_mod* const mod, const mt_ast_sym_table* const tbl) {
    mt_var rst = mt_hash_get(tbl->local_table->hash, buf);
    if (mt_var_is_null(rst)) {
        return rst;
    }
    mt_write_byte(mod, mt_pfx(LD_LOCAL));
    uint16_t mt_al = (uint16_t) rst.data.mt_int;
    mt_write_bytes(mod, &mt_al, sizeof(uint16_t));
    return mt_var_bool(true);
}

#define mt_cgen_var_exists(tgt, mod, tbl) \
    (tbl->local_table->hash != NULL && !mt_var_is_null(mt_cgen_ld_var(tgt, mod, tbl)))

#define mt_cgen_value_in_tbl(tgt, mod,tbl) \
    (mt_cgen_arg_exists(tgt, mod, tbl) || mt_cgen_var_exists(tgt, mod, tbl))

#define mt_cgen_set_locals(ast, mod, op, mt_al) \
    if (mt_ast_fn_access(ast->node.fn, local_table)->hash != NULL && mt_ast_fn_access(ast->node.fn, local_table)->hash->len > 0) { \
        mt_write_byte(mod, mt_pfx(op)); \
        mt_al = (uint16_t) mt_ast_fn_access(ast->node.fn, local_table)->hash->len; \
        mt_write_bytes(mod, &mt_al, sizeof(uint16_t)); \
    }


static mt_var mt_cgen_walk(mt_cgen_state* const state, const mt_ast* const ast, mt_mod* const mod, const mt_ast_sym_table* const tbl);

static inline mt_var mt_cgen_walk_if(mt_ast_op_list* ops, mt_cgen_state* const state, const mt_ast* const ast, mt_mod* const mod) {
    while (ops != NULL) {
        mt_var rst = mt_cgen_walk(state, ops->op, mod, ast->node.if_smt->sym_table);
        mt_cgen_ck_err(rst);
        ops = ops->next;
    }
    return mt_var_bool(true);
}

static inline void mt_cgen_fill_if_jmps(mt_cgen_state* const state, mt_mod* const mod) {
    for (size_t i = 0; i < state->if_len; i++) {
        mt_cgen_walk_jmp_svb(mod, state->if_pos[i]);
    }
    state->if_len = 0;
}

#define mt_cgen_quic_push(mod, CASE, tm, target) \
    case mt_ast(CASE): \
        mt_write_byte(mod, mt_pfx(PUSH)); \
        mt_var_write_bytes(mod, &tm(ast->node.value.target)); \
        break

static inline void mt_cgen_ibuf(const mt_buf* const buf, mt_mod* const mod) {
    mt_write_byte(mod, mt_pfx(IBUF));
    uint32_t str_len = (uint32_t) buf->len;
    mt_write_bytes(mod, &str_len, sizeof(uint32_t));
    mt_write_bytes(mod, buf->data, buf->len);
}

static mt_var mt_cgen_walk(mt_cgen_state* const state, const mt_ast* const ast, mt_mod* const mod, const mt_ast_sym_table* const tbl) {
    if (ast == NULL) {
        return mt_var_bool(true);
    }
    mt_ast_op_list* ops;
    mt_ast_if_cond* conds;
    mt_ast_hash_list* hash_list;
    mt_var rst;
    mt_op op;
    uint8_t mt_f = 0;
    uint16_t mt_al;
    uint8_t* jmp_hdl = NULL;
    uint32_t _bsize; // in bytes
    switch (ast->type) {
        case mt_ast(FN):
            mt_cgen_set_locals(ast, mod, AL, mt_al);
            if (tbl != NULL) {
                jmp_hdl = mt_cgen_walk_jmp_sva(mod, mt_pfx(JMP));
                mt_mod_reg_fn(mod, mod->len);
                mt_f = mod->f_len - 1;
            }
            mt_cgen_ops_list_walk(state, ast, mod, fn, ops_head);
            if (tbl != NULL) {
                mt_write_byte(mod, mt_pfx(RET));
                mt_mod_reg_fne(mod, mt_f, mod->len);
                mt_cgen_walk_jmp_svb(mod, jmp_hdl);
                mt_write_byte(mod, mt_pfx(LD_SELF));
                mt_write_byte(mod, mt_pfx(LD_FN));
                mt_write_bytes(mod, &mt_f, sizeof(uint8_t));
            }
            break;
        case mt_ast(ASSIGN):
            mt_cgen_walk_bop_side(state, ast, mod, tbl, right);
            switch (ast->node.bop->left->type) {
                case mt_ast(VAR):
                    rst = mt_hash_get(tbl->local_table->hash, ast->node.bop->left->node.value.mt_var);
                    if (mt_var_is_null(rst)) {
                        return mt_var_err(mt_err_cgen_tbl());
                    }
                    mt_write_byte(mod, mt_pfx(SV_LOCAL));
                    mt_al = (uint16_t) rst.data.mt_int;
                    mt_write_bytes(mod, &mt_al, sizeof(uint16_t));
                    return mt_var_bool(true);
                case mt_ast(ARG):
                    rst = mt_hash_get(tbl->arg_table->hash, ast->node.bop->left->node.value.mt_var);
                    if (mt_var_is_null(rst)) {
                        return mt_var_err(mt_err_cgen_tbl());
                    }
                    mt_write_byte(mod, mt_pfx(SV_ARG));
                    mt_write_byte(mod, (uint8_t) rst.data.mt_int);
                    return mt_var_bool(true);
                default:
                    break;
            }
            return mt_var_err(mt_err_cgen_assign());
        case mt_ast(VAR):
            rst = mt_cgen_ld_var(ast->node.value.mt_var, mod, tbl);
            if (mt_var_is_null(rst)) {
                return mt_var_err(mt_err_cgen_tbl());
            }
            break;
        case mt_ast(ARG):
            rst = mt_cgen_ld_arg(ast->node.value.mt_var, mod, tbl);
            if (mt_var_is_null(rst)) {
                return mt_var_err(mt_err_cgen_tbl());
            }
            break;
        mt_cgen_quic_push(mod, INT, mt_var_int, mt_int);
        mt_cgen_quic_push(mod, BOOL, mt_var_bool, mt_bool);
        case mt_ast(STR):
            mt_cgen_ibuf(ast->node.value.mt_str, mod);
            mt_write_byte(mod, mt_pfx(ISTR));
            break;
        case mt_ast(HASH):
            mt_write_byte(mod, mt_pfx(IHASH));
            _bsize = ast->node.hash->_bsize;
            mt_write_bytes(mod, &_bsize, sizeof(uint32_t));
            hash_list = ast->node.hash->hash_head;
            while (hash_list != NULL) {
                mt_cgen_ibuf(hash_list->key, mod);
                rst = mt_cgen_walk(state, hash_list->value, mod, ast->node.hash->sym_table);
                mt_cgen_ck_err(rst);
                mt_write_byte(mod, mt_pfx(PHASH));
                hash_list = hash_list->next;
            }
            break;
        case mt_ast(IF):
            conds = ast->node.if_smt->head;
            while (conds != NULL) {
                if (conds->cond == NULL) {
                    if (conds->next != NULL) {
                        return mt_var_err(mt_err_cgen_if_def());
                    }
                    if (conds->body_head != NULL) {
                        rst = mt_cgen_walk_if(conds->body_head, state, ast, mod);
                        mt_cgen_ck_err(rst);
                    }
                    mt_cgen_fill_if_jmps(state, mod);
                    return mt_var_bool(true);
                }
                rst = mt_cgen_walk(state, conds->cond, mod, ast->node.if_smt->sym_table);
                mt_cgen_ck_err(rst);
                jmp_hdl = mt_cgen_walk_jmp_sva(mod, mt_pfx(JMPF));
                rst = mt_cgen_walk_if(conds->body_head, state, ast, mod);
                mt_cgen_ck_err(rst);
                if (conds->next == NULL) {
                    mt_cgen_fill_if_jmps(state, mod);
                    mt_cgen_walk_jmp_svb(mod, jmp_hdl);
                    return mt_var_bool(true);
                }
                mt_cgen_state_push_if(state, mt_cgen_walk_jmp_sva(mod, mt_pfx(JMP)));
                mt_cgen_walk_jmp_svb(mod, jmp_hdl);
                conds = conds->next;
            }
            break;
        mt_cgen_basic_bop_case(state, ast, mod, tbl, EQ);
        mt_cgen_basic_bop_case(state, ast, mod, tbl, OR);
        case mt_ast(CALL):
            mt_cgen_ops_list_walk(state, ast, mod, call, args_head);
            if (ast->node.call->target != NULL && !mt_cgen_value_in_tbl(ast->node.call->target, mod, tbl)) {
                return mt_var_err(mt_err_cgen_tbl());
            }
            op = ast->node.call->target == NULL && ast->node.call->anon == false ? mt_pfx(CALL_SELF) : mt_pfx(CALL);
            mt_write_byte(mod, op);
            mt_write_byte(mod, ast->node.call->arg_count);
            break;
        mt_cgen_basic_bop_case(state, ast, mod, tbl, MUL);
        mt_cgen_basic_bop_case(state, ast, mod, tbl, REM);
        mt_cgen_basic_bop_case(state, ast, mod, tbl, DIV);
        mt_cgen_basic_bop_case(state, ast, mod, tbl, ADD);
        case mt_ast(SUB):
            if (ast->node.bop->left == NULL) {
                // @TODO NEG
            } else {
                mt_cgen_basic_bop(state, ast, mod, tbl, mt_pfx(SUB));
            }
            break;
        mt_cgen_basic_bop_case(state, ast, mod, tbl, WRITE);
        default:
            return mt_var_err(mt_err_ast_undef());
    }
    return mt_var_bool(true);
}

mt_var mt_cgen_build(mt_cgen_state* const state, const mt_ast* const ast, mt_mod* const mod, bool repl) {
    uint8_t mt_f;
    if (repl == false) {
        mt_mod_reg_fn(mod, mod->len);
        mt_f = mod->f_len - 1;
    }
    mt_var rst = mt_cgen_walk(state, ast, mod, NULL);
    mt_cgen_ck_err(rst);
    mt_write_byte(mod, mt_pfx(HALT));
    if (repl == false) {
        mt_mod_reg_fne(mod, mt_f, mod->len);
    }
    return mt_var_bool(true);
}
