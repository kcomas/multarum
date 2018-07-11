
#include "vm.h"

int main(void) {
    mt_mod* mod = mt_mod_init(500);
    mt_vm vm;
    mt_vm_init(&vm);
    mt_vm_free(&vm);
    mt_mod_free(mod);
    return 0;
}
