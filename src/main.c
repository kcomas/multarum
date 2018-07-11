
#include <stdio.h>
#include "vm.h"

int main(void) {
    mt_mod* mod = mt_mod_init(500);
    mt_write_byte(mod, MT_PUSH);
    mt_var_write_bytes(mod, &mt_var_int(3));
    mt_write_byte(mod, MT_PUSH);
    mt_var_write_bytes(mod, &mt_var_int(4));
    mt_write_byte(mod, MT_ADD);
    mt_write_byte(mod, MT_HALT);
    for (size_t i = 0; i < mod->len; i++) {
        printf("%x ", mod->bytes[i]);
    }
    printf("\n");
    mt_vm vm;
    mt_vm_init(&vm, mod);
    mt_var rst = mt_vm_run(&vm);
    printf("%li\n", rst.data.mt_int);
    mt_vm_free(&vm);
    mt_mod_free(mod);
    return 0;
}
