
#include "file.h"
#include "vm.h"
#include "cgen.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage %s file [opts]\n", argv[0]);
        exit(1);
    }

    mt_ctx ctx;
    mt_ctx_init(&ctx, argc, argv);

    mt_buf* to_open = mt_buf_from_c_str(argv[1]);

    mt_var script = mt_file_open(to_open);
    mt_var_debug_print(script);
    printf("\n");

    mt_buf_free(to_open);

    mt_var read_status = mt_read_file_chunk(script.data.mt_file, ctx.read_buf);

    printf("Read Status: ");
    mt_var_debug_print(read_status);
    printf("\n");

    mt_var script_buf = mt_var_buf(ctx.read_buf);
    mt_var_debug_print(script_buf);
    printf("\n");

    mt_token_state token_state;
    mt_token_state_init(&token_state);
    mt_var tokenize_rst = mt_tokenize_buf(&token_state, ctx.read_buf);
    printf("Tokenizer Status: ");
    mt_var_debug_print(tokenize_rst);
    printf("\n");

    mt_var closed = mt_file_close(script.data.mt_file);

    mt_var_debug_print(closed);
    printf("\n");

    mt_token_state_debug_print(&token_state);
    printf("\n");

    mt_ast_state ast_state;
    mt_ast_init(&ast_state);

    mt_var ast_rst = mt_ast_build(&ast_state, token_state.head);
    printf("AST Status: ");
    mt_var_debug_print(ast_rst);
    printf("\n");
    mt_ast_debug_print(ast_state.ast, 0);

    mt_mod* mod = mt_mod_init(MT_MOD_DEFAULT_SIZE, MT_MOD_DEFAULT_FN_SIZE);

    mt_cgen_state cgen_state;
    mt_cgen_state_init(&cgen_state, MT_CGEN_STATE_DEFAULT_IF_SIZE);

    mt_var code_rst = mt_cgen_build(&cgen_state, ast_state.ast, mod);
    mt_cgen_state_free(&cgen_state);
    printf("CGEN Status: ");
    mt_var_debug_print(code_rst);
    printf("\n");

    mt_ast_free(&ast_state);

    mt_token_state_free(&token_state);

    mt_mod_dis(mod);

    exit(1);

    mt_vm vm;
    mt_vm_init(&vm, &ctx, mod);
    printf("Output\n");
    mt_var rst = mt_vm_run(&vm);
    printf("End Output\n");
    printf("Vm Retured Value: ");
    mt_var_debug_print(rst);
    printf("\n");
    mt_vm_free(&vm);
    mt_ctx_free(&ctx);
    return 0;
}
