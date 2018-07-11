
#include "vm.h"

void mt_vm_init(mt_vm* const vm) {
    vm->s_len = 0;
    vm->r_len = 0;
    vm->stack = (mt_var*) malloc(sizeof(mt_var) * MT_DEFAULT_STACK_SIZE);
    vm->rip = (uint8_t**) malloc(sizeof(uint8_t*) * MT_DEFAULT_FRAME_SIZE);
}

void mt_vm_free(mt_vm* const vm) {
    free(vm->stack);
    free(vm->rip);
}
