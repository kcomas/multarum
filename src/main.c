
#include "file.h"
#include "vm.h"

int main(void) {

    mt_var script = mt_open_file("./examples/fib.mul");
    mt_var_debug_print(&script);
    printf("\n");

    mt_mod* mod = mt_mod_init(500, 2);

    uint64_t fib = 30;

    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(fib));
    mt_write_byte(mod, mt_pfx(LD_SELF));
    mt_write_byte(mod, mt_pfx(LD_FN));
    uint8_t fn0 = 0;
    mt_write_bytes(mod, &fn0, sizeof(uint8_t));
    mt_write_byte(mod, mt_pfx(CALL));
    mt_write_byte(mod, 1);
    mt_write_byte(mod, mt_pfx(HALT));

    mt_mod_reg_fn(mod, mod->len);
    mt_write_byte(mod, mt_pfx(LD_ARG));
    mt_write_byte(mod, 0);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(0));
    mt_write_byte(mod, mt_pfx(EQ));
    uint32_t skip0 = 45;
    mt_write_jmp(mod, mt_pfx(JMPF), &skip0);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(0));
    mt_write_byte(mod, mt_pfx(RET));

    mt_write_byte(mod, mt_pfx(LD_ARG));
    mt_write_byte(mod, 0);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(1));
    mt_write_byte(mod, mt_pfx(EQ));
    uint32_t skip1 = 74;
    mt_write_jmp(mod, mt_pfx(JMPF), &skip1);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(1));
    mt_write_byte(mod, mt_pfx(RET));

    mt_write_byte(mod, mt_pfx(LD_ARG));
    mt_write_byte(mod, 0);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(1));
    mt_write_byte(mod, mt_pfx(SUB));
    mt_write_byte(mod, mt_pfx(CALL_SELF));
    mt_write_byte(mod, 1);

    mt_write_byte(mod, mt_pfx(LD_ARG));
    mt_write_byte(mod, 0);
    mt_write_byte(mod, mt_pfx(PUSH));
    mt_var_write_bytes(mod, &mt_var_int(2));
    mt_write_byte(mod, mt_pfx(SUB));
    mt_write_byte(mod, mt_pfx(CALL_SELF));
    mt_write_byte(mod, 1);

    mt_write_byte(mod, mt_pfx(ADD));
    mt_write_byte(mod, mt_pfx(RET));

    mt_mod_dis(mod);

    mt_vm vm;
    mt_vm_init(&vm, mod);
    mt_var rst = mt_vm_run(&vm);
    printf("Rst: %li\n", rst.data.mt_int);
    mt_vm_debug_stack_print(&vm);
    mt_vm_free(&vm);
    return 0;
}
