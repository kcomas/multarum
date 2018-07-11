
#include "vm.h"

void mt_vm_init(mt_vm* const vm, const mt_mod* const mod) {
    vm->s_len = 0;
    vm->r_len = 0;
    vm->stack = (mt_var*) malloc(sizeof(mt_var) * MT_DEFAULT_STACK_SIZE);
    vm->rip = (uint8_t**) malloc(sizeof(uint8_t*) * MT_DEFAULT_FRAME_SIZE);
    vm->rip[vm->r_len++] = mod->bytes;
}

void mt_vm_free(mt_vm* const vm) {
    free(vm->stack);
    free(vm->rip);
}

static void mt_run_op(mt_vm* const vm) {
    int64_t mt_int;
    double mt_float;

    switch (*mt_vm_cur_byte(vm)) {
        case MT_NOP:
            mt_vm_cur_byte(vm)++;
            break;
        case MT_PUSH:
            switch (*++mt_vm_cur_byte(vm)) {
                case MT_NULL:
                    mt_vm_push(vm, mt_var_null);
                    mt_vm_cur_byte(vm)++;
                    break;
                case MT_BOOL:
                    mt_vm_push(vm, mt_var_bool(*++mt_vm_cur_byte(vm)));
                    mt_vm_cur_byte(vm)++;
                    break;
                case MT_CHAR:
                    mt_vm_push(vm, mt_var_char(*++mt_vm_cur_byte(vm)));
                    mt_vm_cur_byte(vm)++;
                    break;
                case MT_INT:
                    mt_vm_cur_byte(vm)++;
                    mt_vm_get_bytes(vm, &mt_int, sizeof(int64_t));
                    mt_vm_push(vm, mt_var_int(mt_int));
                    break;
                case MT_FLOAT:
                    mt_vm_cur_byte(vm)++;
                    mt_vm_get_bytes(vm, &mt_float, sizeof(double));
                    mt_vm_push(vm, mt_var_float(mt_float));
                    break;
            }
            break;
        case MT_ADD:
            // @TODO check types
            mt_vm_prev_stack(vm).data.mt_int += mt_vm_cur_stack(vm).data.mt_int;
            mt_vm_dec_stack(vm);
            mt_vm_cur_byte(vm)++;
            break;
    }
}

mt_var mt_vm_run(mt_vm* const vm) {
    while (*mt_vm_cur_byte(vm) != MT_HALT) {
        mt_run_op(vm);
    }
    if (vm->s_len == 0) {
        return mt_var_null;
    }
    return mt_vm_cur_stack(vm);
}
