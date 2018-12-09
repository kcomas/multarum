
#include "vm.h"

void mt_vm_init(mt_vm* const vm, mt_ctx* const ctx, mt_mod* const mod) {
    vm->mode = mt_pfx(VM_WAIT);
    vm->s_len = 0;
    vm->f_len = 0;
    vm->stack = (mt_var*) malloc(sizeof(mt_var) * MT_DEFAULT_STACK_SIZE);
    vm->rsp = (mt_frame*) malloc(sizeof(mt_frame) * MT_DEFAULT_FRAME_SIZE);
    vm->rsp[vm->f_len].safe = false;
    vm->rsp[vm->f_len].fn = 0;
    vm->rsp[vm->f_len].args = 0;
    vm->rsp[vm->f_len].locals = 0;
    vm->rsp[vm->f_len].rbp = vm->s_len;
    vm->rsp[vm->f_len].mod = mod;
    vm->rsp[vm->f_len++].rip = mod->bytes;
    vm->ctx = ctx;
}

static inline void mt_vm_dec_stack_atomic(mt_vm* const vm) {
    vm->s_len--;
}

static inline void mt_vm_dec_stack(mt_vm* const vm) {
    mt_var_free(vm->stack[vm->s_len - 1]);
    mt_vm_dec_stack_atomic(vm);
}

void mt_vm_free(mt_vm* const vm) {
    while (vm->s_len > 0) {
        mt_vm_dec_stack(vm);
    }
    free(vm->stack);
    mt_mod_free(mt_vm_cur_mod(vm));
    mt_vm_dec_frame(vm)
    free(vm->rsp);
}

static inline void mt_vm_inc_ref(mt_vm* const vm) {
    mt_var_inc_ref(mt_vm_cur_stack(vm));
}

static inline bool mt_vm_stack_type_cmp(mt_vm* const vm, mt_var_type type) {
    return mt_vm_prev_stack(vm).type == type && mt_vm_cur_stack(vm).type == type;
}

static inline void mt_vm_push(mt_vm* const vm, mt_var var) {
    vm->stack[vm->s_len++] = var;
}

static inline void mt_vm_update(mt_vm* const vm, mt_var var) {
    vm->stack[vm->s_len - 1] = var;
}

static inline void mt_vm_get_bytes(mt_vm* restrict vm, void* restrict dest, size_t size) {
    memcpy(dest, mt_vm_cur_byte(vm), size);
    mt_vm_cur_byte(vm) += size;
}

static inline void mt_vm_jmp(mt_vm* const vm, uint32_t mt_jmp) {
    mt_vm_get_bytes(vm, &mt_jmp, sizeof(uint32_t));
    mt_vm_cur_byte(vm) = mt_vm_cur_mod(vm)->bytes + mt_jmp;
}

static void mt_vm_call(mt_vm* const vm, mt_mod* const new_mod, size_t new_idx, size_t new_base) {
    mt_vm_cur_byte(vm)++;
    uint8_t num_args = *mt_vm_cur_byte(vm)++;
    mt_vm_inc_frame(vm);
    mt_vm_cur_safe(vm) = false;
    mt_vm_cur_fn(vm) = new_idx;
    mt_vm_cur_args(vm) = num_args;
    mt_vm_cur_locals(vm) = 0;
    mt_vm_cur_base(vm) = new_base - num_args;
    mt_vm_cur_mod(vm) = new_mod;
    mt_vm_cur_byte(vm) = &mt_vm_cur_mod(vm)->bytes[mt_vm_cur_mod(vm)->fns[mt_vm_cur_fn(vm)]];
}

static void mt_vm_ret(mt_vm* const vm) {
    if (mt_vm_cur_base(vm) != vm->s_len) {
        mt_var* mt_ret = &mt_vm_cur_stack(vm);
        mt_vm_dec_stack_atomic(vm);
        while (vm->s_len >= mt_vm_cur_base(vm)) {
            mt_vm_dec_stack(vm);
        }
        mt_vm_push(vm, *mt_ret);
    } else {
        mt_vm_push(vm, mt_var_null());
    }
    mt_mod_free(mt_vm_cur_mod(vm));
    mt_vm_dec_frame(vm)
}

static void mt_run_op(mt_vm* const vm) {
    bool mt_bool;
    uint8_t mt_fn, mt_arg;
    int8_t char_conts;
    int64_t mt_int;
    double mt_float;
    uint32_t mt_jmp = 0;
    uint32_t str_len;
    uint16_t mt_al;
    mt_buf* mt_buf;
    mt_char mt_char_parts = mt_char_init(0, 0, 0, 0);
    switch (*mt_vm_cur_byte(vm)) {
        case mt_pfx(NOP):
            mt_vm_cur_byte(vm)++;
            break;
        case mt_pfx(AL):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_al, sizeof(uint16_t));
            vm->s_len += mt_al;
            mt_vm_cur_locals(vm) = mt_al; // for repl
            break;
        case mt_pfx(PUSH):
            switch (*++mt_vm_cur_byte(vm)) {
                case mt_pfx(NULL):
                    mt_vm_push(vm, mt_var_null());
                    mt_vm_cur_byte(vm)++;
                    break;
                case mt_pfx(BOOL):
                    mt_vm_push(vm, mt_var_bool(*++mt_vm_cur_byte(vm)));
                    mt_vm_cur_byte(vm)++;
                    break;
                case mt_pfx(CHAR):
                    mt_char_parts.a = *++mt_vm_cur_byte(vm);
                    char_conts = mt_char_cont(mt_char_parts.a);
                    if (char_conts < 0) {
                        // @TODO handle error
                    }
                    if (char_conts >= 1) {
                        mt_char_parts.b = *++mt_vm_cur_byte(vm);
                        if (char_conts >= 2) {
                            mt_char_parts.c = *++mt_vm_cur_byte(vm);
                            if (char_conts == 3) {
                                mt_char_parts.d = *++mt_vm_cur_byte(vm);
                            }
                        }
                    }
                    mt_vm_push(vm, mt_var_char(mt_char_parts));
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
        case mt_pfx(ISTR):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &str_len, sizeof(uint32_t));
            mt_buf = mt_buf_init((size_t) str_len);
            for (size_t i = 0; i < mt_buf->_size; i++) {
                mt_buf->data[mt_buf->len++] = *mt_vm_cur_byte(vm)++;
            }
            mt_vm_push(vm, mt_var_str(mt_str_init(mt_buf)));
            break;
        mt_vm_math_case(MUL, vm, *=);
        case mt_pfx(MOD):
            if (mt_vm_stack_type_cmp(vm, mt_pfx(INT))) {
                mt_vm_math_op(vm, %=, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, mt_pfx(FLOAT))) {
                // @TODO double rem
            } else {
                // @TODO handle err
            }
            break;
        mt_vm_math_case(DIV, vm, /=);
        mt_vm_math_case(ADD, vm, +=);
        mt_vm_math_case(SUB, vm, -=);
        case mt_pfx(EQ):
            if (mt_vm_stack_type_cmp(vm, mt_pfx(INT))) {
               mt_vm_eq_op(vm, mt_bool, mt_int);
            } else if (mt_vm_stack_type_cmp(vm, mt_pfx(FLOAT))) {
               mt_vm_eq_op(vm, mt_bool, mt_float);
            } else {
                // @TODO handle error
                break;
            }
            mt_vm_push(vm, mt_var_bool(mt_bool));
            break;
        case mt_pfx(OR):
            mt_vm_cur_byte(vm)++;
            if (mt_vm_stack_type_cmp(vm, mt_pfx(BOOL))) {
                if (mt_vm_cur_stack(vm).data.mt_bool == true || mt_vm_prev_stack(vm).data.mt_bool == true) {
                    mt_bool = true;
                } else {
                    mt_bool = false;
                }
            } else {
                // @ TODO handle error
                break;
            }
            mt_vm_dec_stack_atomic(vm);
            mt_vm_dec_stack_atomic(vm);
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
            mt_vm_push(vm, mt_var_mod(mt_vm_cur_mod(vm)));
            mt_vm_cur_byte(vm)++;
            mt_vm_inc_ref(vm);
            break;
        case mt_pfx(LD_FN):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_fn, sizeof(uint8_t));
            mt_vm_update(vm, mt_var_fn(mt_vm_cur_stack(vm).data.mt_mod, mt_fn));
            break;
        case mt_pfx(LD_ARG):
            mt_vm_cur_byte(vm)++;
            mt_arg = *mt_vm_cur_byte(vm)++;
            mt_vm_push(vm, vm->stack[mt_vm_cur_base(vm) + mt_arg - 1]);
            mt_vm_inc_ref(vm);
            break;
        case mt_pfx(SV_ARG):
            mt_vm_cur_byte(vm)++;
            mt_arg = *mt_vm_cur_byte(vm)++;
            vm->stack[mt_vm_cur_base(vm) + mt_arg - 1] = mt_vm_cur_stack(vm);
            mt_vm_inc_ref(vm);
            break;
        case mt_pfx(LD_LOCAL):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_al, sizeof(uint16_t));
            mt_vm_push(vm, vm->stack[mt_vm_cur_base(vm) + mt_vm_cur_args(vm) + mt_al]);
            mt_vm_inc_ref(vm);
            break;
        case mt_pfx(SV_LOCAL):
            mt_vm_cur_byte(vm)++;
            mt_vm_get_bytes(vm, &mt_al, sizeof(uint16_t));
            vm->stack[mt_vm_cur_base(vm) + mt_vm_cur_args(vm) + mt_al] = mt_vm_cur_stack(vm);
            mt_vm_inc_ref(vm);
            break;
        case mt_pfx(CALL):
            if (mt_vm_cur_stack(vm).type != mt_pfx(FN)) {
             // @TODO handle error
            }
            mt_vm_call(vm, mt_vm_cur_stack(vm).data.mt_mod, mt_vm_cur_stack(vm).meta.fn_idx, vm->s_len);
            mt_vm_dec_stack_atomic(vm);
            break;
        case mt_pfx(CALL_SELF):
            mt_vm_call(vm, mt_vm_cur_frame(vm).mod, mt_vm_cur_frame(vm).fn, vm->s_len + 1);
            mt_vm_cur_mod(vm)->ref_count++;
            break;
        case mt_pfx(WRITE):
            mt_vm_cur_byte(vm)++;
            if (mt_var_is_int(mt_vm_prev_stack(vm))) {
                if (mt_vm_prev_stack(vm).data.mt_int == 1) {
                    // @TODO remove new lines and write to write buffer
                    mt_var_debug_print(mt_vm_cur_stack(vm));
                }
            } else {
                // @TODO handle error
            }
            mt_vm_dec_stack(vm);
            mt_vm_dec_stack(vm);
            break;
        case mt_pfx(RET):
            mt_vm_ret(vm);
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
        return mt_var_null();
    }
    return mt_vm_cur_stack(vm);
}

void mt_vm_debug_stack_print(const mt_vm* const vm) {
    for (size_t i = 0; i < vm->s_len; i++) {
        mt_var_debug_print(vm->stack[i]);
        printf("\n");
    }
}
