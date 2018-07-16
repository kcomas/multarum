
#include "multarum.h"

int main(void) {
    mt_mod* mod = mt_mod_init(500, 2);

    uint32_t main_jmp = 22;
    mt_write_jmp(mod, &main_jmp);

    mt_mod_reg_fn(mod, mod->len);
    mt_write_byte(mod, MT_LD_ARG);
    mt_write_byte(mod, 0);
    mt_write_byte(mod, MT_LD_ARG);
    mt_write_byte(mod, 1);
    mt_write_byte(mod, MT_ADD);
    mt_write_byte(mod, MT_PUSH);
    mt_var_write_bytes(mod, &mt_var_int(2));
    mt_write_byte(mod, MT_SUB);
    mt_write_byte(mod, MT_RET);

    mt_write_byte(mod, MT_PUSH);
    mt_var_write_bytes(mod, &mt_var_int(3));
    mt_write_byte(mod, MT_PUSH);
    mt_var_write_bytes(mod, &mt_var_int(4));

    mt_write_byte(mod, MT_LD_SELF);
    mt_write_byte(mod, MT_LD_FN);
    uint32_t fn0 = 0;
    mt_write_bytes(mod, &fn0, sizeof(uint32_t));
    mt_write_byte(mod, MT_CALL);
    mt_write_byte(mod, 2);
    mt_write_byte(mod, MT_HALT);

    mt_mod_dis(mod);

    mt_vm vm;
    mt_vm_init(&vm, mod);
    mt_var rst = mt_vm_run(&vm);
    printf("Rst: %li\n", rst.data.mt_int);
    mt_vm_debug_stack_print(&vm);
    mt_vm_free(&vm);
    mt_mod_free(mod);
    return 0;
}
