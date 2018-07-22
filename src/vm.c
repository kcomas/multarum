
#include "vm.h"

void mt_vm_init(mt_vm* const vm, mt_mod* const mod) {
    vm->mode = mt_pfx(VM_WAIT);
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
        case mt_pfx(MODULE):
        case mt_pfx(FN):
            mt_mod_free(mt_vm_cur_stack(vm).data.mt_mod);
            break;
        default:
            break;
    }
}

static void mt_run_op(mt_vm* const vm) {
    bool mt_bool;
    uint8_t num_args, mt_arg;
    int64_t mt_int;
    double mt_float;
    uint32_t mt_jmp, mt_fn;
    mt_var mt_ret;

    switch (*mt_vm_cur_byte(vm)) {
        case mt_pfx(NOP):
            mt_vm_cur_byte(vm)++;
            break;
        case mt_pfx(PUSH):
            switch (*++mt_vm_cur_byte(vm)) {
                case mt_pfx(NULL):
                    mt_vm_push(vm, mt_var_null);
                    mt_vm_cur_byte(vm)++;
                    break;
                case mt_pfx(BOOL):
                    mt_vm_push(vm, mt_var_bool(*++mt_vm_cur_byte(vm)));
                    mt_vm_cur_byte(vm)++;
                    break;
                case mt_pfx(CHAR):
                    mt_vm_push(vm, mt_var_char(*++mt_vm_cur_byte(vm)));
                    mt_vm_cur_byte(vm)++;
                    break;
                case mt_pfx(INT):
                    mt_vm_cur_byte(vm)++;
                    mt_vm_get_bytes(vm, &mt_int, sizeof(int64_t));
                    mt_vm_push(vm, mt_var_int(mt_int));
                    break;
                case mt_pfx(FLOAT):
                    mt_vm_cur_byte(vm)++;
                    mt_vm_get_bytes(vm, &mt_float, sizeof(double));
                    mt_vm_push(vm, mt_var_float(mt_float));
                    break;
            }
            break;
        case mt_pfx(ADD):
            if (mt_vm_stack_type_cmp(vm, mt_pfx(INT))) {
                mt_vm_math_op(vm, +=, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, mt_pfx(FLOAT))) {
                mt_vm_math_op(vm, +=, mt_float);
            }
            break;
        case mt_pfx(SUB):
            if (mt_vm_stack_type_cmp(vm, mt_pfx(INT))) {
                mt_vm_math_op(vm, -=, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, mt_pfx(FLOAT))) {
                mt_vm_math_op(vm, -=, mt_float);
            }
            break;
        case mt_pfx(EQ):
            if (mt_vm_stack_type_cmp(vm, mt_pfx(INT))) {
               mt_vm_eq_op(vm, mt_bool, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, mt_pfx(FLOAT))) {
               mt_vm_eq_op(vm, mt_bool, mt_float);
            } else {
                mt_bool = false;
            }
            mt_vm_push(vm, mt_var_bool(mt_bool));
            break;
        case mt_pfx(JMPF):
            mt_vm_cur_byte(vm)++;
            mt_bool = mt_var_as_bool(&mt_vm_cur_stack(vm));
            if (!mt_bool) {
                mt_vm_jmp(vm, mt_jmp);
            } else {
                mt_vm_cur_byte(vm) += sizeof(uint32_t);
            }
            mt_vm_dec_stack(vm);
            break;
        case mt_pfx(JMP):
            mt_vm_cur_byte(vm)++;
            mt_vm_jmp(vm, mt_jmp);
            break;
        case mt_pfx(LD_SELF):
            mt_vm_cur_mod(vm)->ref_count++;
            mt_vm_push(vm, mt_var_mod(mt_vm_cur_mod(vm)));
            mt_vm_cur_byte(vm)++;
            break;
        case mt_pfx(LD_FN):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_fn, sizeof(uint32_t));
            mt_vm_push(vm, mt_var_fn(mt_vm_cur_stack(vm).data.mt_mod, mt_fn));
            mt_vm_dec_stack_atomic(vm);
            break;
        case mt_pfx(LD_ARG):
            mt_vm_cur_byte(vm)++;
            mt_arg = *mt_vm_cur_byte(vm)++;
            mt_vm_push(vm, vm->stack[mt_vm_cur_base(vm) + mt_arg - 1]);
            break;
        case mt_pfx(CALL):
            mt_vm_cur_byte(vm)++;
            num_args = *mt_vm_cur_byte(vm)++;
            mt_vm_inc_frame(vm);
            mt_vm_cur_base(vm) = vm->s_len - num_args;
            mt_vm_cur_mod(vm) = mt_vm_cur_stack(vm).data.mt_mod;
            mt_vm_cur_byte(vm) = &mt_vm_cur_mod(vm)->bytes[mt_vm_cur_mod(vm)->fns[mt_vm_cur_stack(vm).fn_idx]];
            mt_vm_dec_stack_atomic(vm);
            break;
        case mt_pfx(RET):
            if (mt_vm_cur_base(vm) != vm->s_len) {
                mt_ret = mt_vm_cur_stack(vm);
                mt_vm_dec_stack_atomic(vm);
                while (vm->s_len >= mt_vm_cur_base(vm)) {
                    mt_vm_dec_stack(vm);
                }
            } else {
                mt_ret = mt_var_null;
            }
            mt_vm_push(vm, mt_ret);
            mt_mod_free(mt_vm_cur_mod(vm));
            mt_vm_dec_frame(vm);
            break;
        case mt_pfx(HALT):
            vm->mode = mt_pfx(VM_WAIT);
            break;
    }
}

mt_var mt_vm_run(mt_vm* const vm) {
    vm->mode = mt_pfx(VM_RUN);
    while (vm->mode == mt_pfx(VM_RUN)) {
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
