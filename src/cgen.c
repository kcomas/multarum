
#include "cgen.h"

#define mt_cgen_ck_err(rst) \
    if (mt_var_is_err(rst)) { \
        return rst; \
    }

#define mt_cgen_walk_bop_side(ast, mod, tbl, side) \
    rst = mt_cgen_walk(ast->node.bop->side, mod, tbl); \
    mt_cgen_ck_err(rst)

#define mt_cgen_basic_bop(ast, mod, tbl, op) \
    mt_cgen_walk_bop_side(ast, mod, tbl, left); \
    mt_cgen_walk_bop_side(ast, mod, tbl, right); \
    mt_write_byte(mod, op)

#define mt_cgen_basic_bop_case(ast, mod, tbl, op) \
    case mt_ast(op): \
        mt_cgen_basic_bop(ast, mod, tbl, mt_pfx(op)); \
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

#define mt_cgen_ops_list_walk(ast, mod, target, head) \
    ops = ast->node.target->head; \
    while (ops != NULL) { \
        rst = mt_cgen_walk(ops->op, mod, ast->node.target->sym_table); \
        mt_cgen_ck_err(rst); \
        ops = ops->next; \
    }

static mt_var mt_cgen_walk(const mt_ast* const ast, mt_mod* const mod, const mt_ast_sym_table* const tbl) {
    if (ast == NULL) {
        return mt_var_bool(true);
    }
    mt_ast_op_list* ops;
    mt_ast_if_cond* conds;
    mt_var rst;
    mt_op op;
    uint8_t mt_arg;
    uint8_t mt_f;
    uint16_t mt_al;
    uint8_t* jmp_hdl = NULL;
    switch (ast->type) {
        case mt_ast(FN):
            if (mt_ast_fn_access(ast->node.fn, local_table).hash != NULL && mt_ast_fn_access(ast->node.fn, local_table).hash->len > 0) {
                mt_write_byte(mod, mt_pfx(AL));
                mt_al = (uint16_t) mt_ast_fn_access(ast->node.fn, local_table).hash->len;
                mt_write_bytes(mod, &mt_al, sizeof(uint16_t));
            }
            if (tbl != NULL) {
                jmp_hdl = mt_cgen_walk_jmp_sva(mod, mt_pfx(JMP));
                mt_mod_reg_fn(mod, mod->len);
            }
            mt_cgen_ops_list_walk(ast, mod, fn, ops_head);
            if (tbl != NULL) {
                mt_write_byte(mod, mt_pfx(RET));
                mt_mod_reg_fne(mod, mod->len);
                mt_cgen_walk_jmp_svb(mod, jmp_hdl);
                mt_write_byte(mod, mt_pfx(LD_SELF));
                mt_write_byte(mod, mt_pfx(LD_FN));
                mt_f = mod->f_len - 1;
                mt_write_bytes(mod, &mt_f, sizeof(uint8_t));
            }
            break;
        case mt_ast(ASSIGN):
            mt_cgen_walk_bop_side(ast, mod, tbl, right);
            switch (ast->node.bop->left->type) {
                case mt_ast(VAR):
                    rst = mt_hash_get(tbl->local_table.hash, ast->node.bop->left->node.value.mt_var);
                    if (mt_var_is_null(rst)) {
                        return mt_var_err(mt_err_cgen_tbl());
                    }
                    mt_write_byte(mod, mt_pfx(SV_LOCAL));
                    mt_al = (uint16_t) rst.data.mt_int;
                    mt_write_bytes(mod, &mt_al, sizeof(uint16_t));
                    return mt_var_bool(true);
                default:
                    break;
            }
            return mt_var_err(mt_err_cgen_assign());
        case mt_ast(ARG):
            rst = mt_hash_get(tbl->arg_table.hash, ast->node.value.mt_var);
            if (mt_var_is_null(rst)) {
                return mt_var_err(mt_err_cgen_tbl());
            }
            mt_write_byte(mod, mt_pfx(LD_ARG));
            mt_arg = (uint8_t) rst.data.mt_int;
            mt_write_bytes(mod, &mt_arg, sizeof(uint8_t));
            break;
        case mt_ast(INT):
            mt_write_byte(mod, mt_pfx(PUSH));
            mt_var_write_bytes(mod, &mt_var_int(ast->node.value.mt_int));
            break;
        case mt_ast(IF):
            conds = ast->node.if_smt->head;
            while (conds != NULL) {
                if (conds->cond == NULL) {
                    if (conds->next != NULL) {
                        return mt_var_err(mt_err_cgen_if_def());
                    }
                    rst = mt_cgen_walk(conds->body, mod, ast->node.if_smt->sym_table);
                    mt_cgen_ck_err(rst);
                    // @TODO fill jmps
                    return mt_var_bool(true);
                }
                rst = mt_cgen_walk(conds->cond, mod, ast->node.if_smt->sym_table);
                mt_cgen_ck_err(rst);
                jmp_hdl = mt_cgen_walk_jmp_sva(mod, mt_pfx(JMPF));
                rst = mt_cgen_walk(conds->body, mod, ast->node.if_smt->sym_table);
                mt_cgen_ck_err(rst);
                // @TODO write jmp to end of if
                (void) mt_cgen_walk_jmp_sva(mod, mt_pfx(JMP));
                mt_cgen_walk_jmp_svb(mod, jmp_hdl);
                conds = conds->next;
            }
            break;
        mt_cgen_basic_bop_case(ast, mod, tbl, EQ);
        mt_cgen_basic_bop_case(ast, mod, tbl, OR);
        case mt_ast(CALL):
            // @TODO load var or arg
            mt_cgen_ops_list_walk(ast, mod, call, args_head);
            op = ast->node.call->target == NULL ? mt_pfx(CALL_SELF) : mt_pfx(CALL);
            mt_write_byte(mod, op);
            mt_write_byte(mod, ast->node.call->arg_count);
            break;
        mt_cgen_basic_bop_case(ast, mod, tbl, ADD);
        mt_cgen_basic_bop_case(ast, mod, tbl, SUB);
        default:
            return mt_var_err(mt_err_ast_undef());
    }
    return mt_var_bool(true);
}

mt_var mt_cgen_build(const mt_ast* const ast, mt_mod* const mod) {
    mt_var rst = mt_cgen_walk(ast, mod, NULL);
    // mt_cgen_ck_err(rst);
    mt_write_byte(mod, mt_pfx(HALT));
    return mt_var_bool(true);
}
