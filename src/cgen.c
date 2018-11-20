
#include "cgen.h"

#define mt_cgen_walk_bop_side(ast, mod, tbl, side) \
    rst = mt_cgen_walk(ast->node.bop->side, mod, tbl); \
    if (mt_var_is_err(rst)) { \
        return rst; \
    } \

static mt_var mt_cgen_walk(const mt_ast* const ast, mt_mod* const mod, const mt_ast_sym_table* const tbl) {
    mt_ast_op_list* ops;
    mt_var rst;
    uint16_t mt_al;
    switch (ast->type) {
        case mt_ast(FN):
            if (mt_ast_fn_access(ast->node.fn, local_table).hash != NULL && mt_ast_fn_access(ast->node.fn, local_table).hash->len > 0) {
                mt_write_byte(mod, mt_pfx(AL));
                mt_al = (uint16_t) mt_ast_fn_access(ast->node.fn, local_table).hash->len;
                mt_write_bytes(mod, &mt_al, sizeof(uint16_t));
            }
            ops = ast->node.fn->ops_head;
            while (ops != NULL) {
                rst = mt_cgen_walk(ops->op, mod, ast->node.fn->sym_table);
                if (mt_var_is_err(rst)) {
                    return rst;
                }
                ops = ops->next;
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
        case mt_ast(INT):
            mt_write_byte(mod, mt_pfx(PUSH));
            mt_var_write_bytes(mod, &mt_var_int(ast->node.value.mt_int));
            break;
        default:
            return mt_var_err(mt_err_ast_undef());
    }
    return mt_var_bool(true);
}

mt_var mt_cgen_build(const mt_ast* const ast, mt_mod* const mod) {
    mt_var rst = mt_cgen_walk(ast, mod, NULL);
    if (mt_var_is_err(rst)) {
        // @TODO return rst;
    }
    mt_write_byte(mod, mt_pfx(HALT));
    return mt_var_bool(true);
}
