
#include "vm.h"

void mt_vm_init(mt_vm* const vm, mt_mod* const mod) {
    vm->s_len = 0;
    vm->f_len = 0;
    vm->stack = (mt_var*) malloc(sizeof(mt_var) * MT_DEFAULT_STACK_SIZE);
    vm->rsp = (mt_frame*) malloc(sizeof(mt_frame) * MT_DEFAULT_FRAME_SIZE);
    vm->rsp[vm->f_len].rbp = vm->s_len;
    vm->rsp[vm->f_len].mod = mod;
    vm->rsp[vm->f_len++].rip = mod->bytes;
}

void mt_vm_free(mt_vm* const vm) {
    free(vm->stack);
    free(vm->rsp);
}

void mt_vm_dec_stack(mt_vm* const vm) {
    switch (vm->stack[--vm->s_len].type) {
        case MT_MODULE:
        case MT_FN:
            mt_mod_free(mt_vm_cur_stack(vm).data.mt_mod);
            break;
        default:
            break;
    }
}

static void mt_run_op(mt_vm* const vm) {
    uint8_t num_args, mt_arg;
    int64_t mt_int;
    double mt_float;
    uint32_t mt_jmp, mt_fn;

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
            if (mt_vm_stack_type_cmp(vm, MT_INT)) {
                mt_vm_math_op(vm, +=, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, MT_FLOAT)) {
                mt_vm_math_op(vm, +=, mt_float);
            }
            break;
        case MT_SUB:
            if (mt_vm_stack_type_cmp(vm, MT_INT)) {
                mt_vm_math_op(vm, -=, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, MT_FLOAT)) {
                mt_vm_math_op(vm, -=, mt_float);
            }
            break;
        case MT_JMP:
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_jmp, sizeof(uint32_t));
            mt_vm_cur_byte(vm) = mt_vm_cur_mod(vm)->bytes + mt_jmp;
            break;
        case MT_LD_SELF:
            mt_vm_cur_mod(vm)->ref_count++;
            mt_vm_push(vm, mt_var_mod(mt_vm_cur_mod(vm)));
            mt_vm_cur_byte(vm)++;
            break;
        case MT_LD_FN:
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_fn, sizeof(uint32_t));
            mt_vm_push(vm, mt_var_fn(mt_vm_cur_stack(vm).data.mt_mod, mt_fn));
            mt_vm_dec_stack_atomic(vm);
            break;
        case MT_LD_ARG:
            mt_vm_cur_byte(vm)++;
            mt_arg = *mt_vm_cur_byte(vm)++;
            mt_vm_push(vm, vm->stack[mt_vm_cur_base(vm) + mt_arg - 1]);
            break;
        case MT_CALL:
            mt_vm_cur_byte(vm)++;
            num_args = *mt_vm_cur_byte(vm)++;
            mt_vm_inc_frame(vm);
            mt_vm_cur_base(vm) = vm->s_len - num_args;
            mt_vm_cur_mod(vm) = mt_vm_cur_stack(vm).data.mt_mod;
            mt_vm_cur_byte(vm) = &mt_vm_cur_mod(vm)->bytes[mt_vm_cur_mod(vm)->fns[mt_vm_cur_stack(vm).fn_idx]];
            mt_vm_dec_stack_atomic(vm);
            break;
        case MT_RET:
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

void mt_vm_debug_stack_print(const mt_vm* const vm) {
    for (size_t i = 0; i < vm->s_len; i++) {
        mt_var_debug_print(&vm->stack[i]);
        printf("\n");
    }
}
