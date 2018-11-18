
#include "cgen.h"

#define mt_cgen_walk_side(ast, modd, tbl, side) \
    rst = mt_cgen_walk(ast->node.bop->side, modd, tbl); \
    if (mt_var_is_err(rst)) { \
        return rst; \
    } \

static mt_var mt_cgen_walk(const mt_ast* const ast, mt_mod** const modd, const mt_ast_sym_table* const tbl) {
    mt_mod* mod = *modd;
    mt_ast_op_list* ops;
    mt_var rst;
    switch (ast->type) {
        case mt_ast(FN):
            ops = ast->node.fn->ops_head;
            while (ops != NULL) {
                rst = mt_cgen_walk(ops->op, modd, ast->node.fn->sym_table);
                if (mt_var_is_err(rst)) {
                    return rst;
                }
                ops = ops->next;
            }
            break;
        case mt_ast(ASSIGN):
            mt_cgen_walk_side(ast, modd, tbl, right);
            break;
        case mt_ast(INT):
            mt_write_byte(mod, mt_pfx(PUSH));
            mt_var_write_bytes(mod, &mt_var_int(ast->node.value.mt_int));
            break;
        default:
            return mt_var_err(mt_err_ast_undef());
    }
    return mt_var_bool(true);
}

mt_var mt_cgen_build(const mt_ast* const ast, mt_mod** const modd) {
    mt_mod* mod = *modd;
    if (mt_ast_fn_access(ast->node.fn, local_table).hash->len > 0) {
        mt_write_byte(mod, mt_pfx(AL));
        uint16_t al = (uint16_t) mt_ast_fn_access(ast->node.fn, local_table).hash->len;
        mt_write_bytes(mod, &al, sizeof(uint16_t));
    }
    mt_var rst = mt_cgen_walk(ast, modd, NULL);
    if (mt_var_is_err(rst)) {
        // @TODO return rst;
    }
    mt_write_byte(mod, mt_pfx(HALT));
    return mt_var_bool(true);
}
