
#include "repl.h"

#define mt_ck_err(rst) \
    if (mt_var_is_err(rst)) { \
        mt_var_debug_print(rst); \
        exit(1); \
    }

void mt_repl_run(int argc, char** argv) {
    mt_repl repl;
    mt_ctx_init(&repl.ctx, argc, argv);
    mt_ast_table* global_table = mt_ast_empty_table();
    repl.sym_table = mt_ast_init_sym_table(global_table, true);
    mt_mod* mod = mt_mod_init(MT_MOD_DEFAULT_SIZE, MT_MOD_DEFAULT_FN_SIZE);
    mod->f_len++; // zero is reserved for the mod
    mt_vm_init(&repl.vm, &repl.ctx, mod);
    for (;;) {
        mt_buf* read_buf = repl.ctx.read_buf;
        mt_token_state token_state;
        mt_token_state_init(&token_state);
        printf(" ");
        fflush(stdout);
        uint32_t total = 0;
        while ((total = read(0, read_buf->data + read_buf->len, 1)) > 0) {
            read_buf->len++;
            if (mt_buf_tail(read_buf) == '\n') {
                break;
            }
        }
        if (total == 0) {
            putchar('\n');
            break;
        }
        mt_var tokenize_rst = mt_tokenize_buf(&token_state, read_buf);
        mt_ck_err(tokenize_rst);
        read_buf->len = 0;
        mt_ast_state ast_state;
        mt_ast_init(&ast_state, repl.sym_table);
        mt_var ast_rst = mt_ast_build(&ast_state, token_state.head);
        mt_ck_err(ast_rst);
        mt_cgen_state cgen_state;
        mt_cgen_state_init(&cgen_state, MT_CGEN_STATE_DEFAULT_IF_SIZE);
        mt_var code_rst = mt_cgen_build(&cgen_state, ast_state.ast, mod, true);
        mt_ck_err(code_rst);
        mt_cgen_state_free(&cgen_state);
        mt_ast_free(&ast_state);
        mt_token_state_free(&token_state);
        mt_var vm_rst = mt_vm_run(&repl.vm);
        mt_ck_err(vm_rst);
        printf("\n ");
        mt_var_debug_print(repl.vm.stack[repl.vm.s_len - 1]);
        printf("\n");
        fflush(stdout);
        repl.vm.rsp[repl.vm.f_len - 1].rip = mod->bytes + mod->len;
    }
    mt_vm_free(&repl.vm);
    mt_ctx_free(&repl.ctx);
    mt_ast_free_sym_table(repl.sym_table, true);
    free(global_table);
}
