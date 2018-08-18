
#ifndef MT_VM_H
#define MT_VM_H

#include <stdlib.h>
#include <stdio.h>
#include "common.h"
#include "op.h"
#include "var.h"
#include "frame.h"
#include "ctx.h"

#ifndef MT_DEFAULT_STACK_SIZE
#   define MT_DEFAULT_STACK_SIZE 2000
#endif

#ifndef MT_DEFAULT_FRAME_SIZE
#   define MT_DEFAULT_FRAME_SIZE 500
#endif

typedef enum {
    mt_pfx(VM_WAIT),
    mt_pfx(VM_RUN),
    mt_pfx(VM_ERR)
} mt_vm_mode;

typedef struct {
    mt_vm_mode mode;
    size_t s_len, f_len;
    mt_var* stack;
    mt_frame* rsp;
    mt_ctx* ctx;
} mt_vm;

#define mt_vm_prev_frame(vm) vm->rsp[vm->f_len - 2]

#define mt_vm_cur_frame(vm) vm->rsp[vm->f_len - 1]

#define mt_vm_cur_safe(vm) mt_vm_cur_frame(vm).safe

#define mt_vm_cur_fn(vm) mt_vm_cur_frame(vm).fn

#define mt_vm_cur_byte(vm) mt_vm_cur_frame(vm).rip

#define mt_vm_cur_mod(vm) mt_vm_cur_frame(vm).mod

#define mt_vm_cur_base(vm) mt_vm_cur_frame(vm).rbp

#define mt_vm_inc_frame(vm) vm->f_len++;

#define mt_vm_dec_frame(vm) vm->f_len--;

#define mt_vm_cur_stack(vm) vm->stack[vm->s_len - 1]

#define mt_vm_prev_stack(vm) vm->stack[vm->s_len - 2]

#define mt_vm_stack_type_cmp(vm, var_type) \
    (mt_vm_prev_stack(vm).type == var_type) && (mt_vm_cur_stack(vm).type == var_type)

#define mt_vm_push(vm, var) vm->stack[vm->s_len++] = var;

#define mt_vm_get_bytes(vm, dest, size) \
    memcpy(dest, mt_vm_cur_byte(vm), size); \
    mt_vm_cur_byte(vm) += size

#define mt_vm_math_op(vm, op, v_data) \
    mt_vm_prev_stack(vm).data.v_data op mt_vm_cur_stack(vm).data.v_data; \
    mt_vm_dec_stack(vm); \
    mt_vm_cur_byte(vm)++

#define mt_vm_eq_op(vm, target, v_data) \
    target = mt_vm_prev_stack(vm).data.v_data == mt_vm_cur_stack(vm).data.v_data; \
    mt_vm_dec_stack(vm); \
    mt_vm_dec_stack(vm); \
    mt_vm_cur_byte(vm)++

#define mt_vm_jmp(vm, mt_jmp) \
    mt_vm_get_bytes(vm, &mt_jmp, sizeof(uint32_t)); \
    mt_vm_cur_byte(vm) = mt_vm_cur_mod(vm)->bytes + mt_jmp

void mt_vm_init(mt_vm* const vm, mt_ctx* const ctx, mt_mod* const mod);

void mt_vm_free(mt_vm* const vm);

#define mt_vm_dec_stack_atomic(vm) vm->s_len--;

#define mt_vm_err_handle(vm, type, msg) \
    if (!mt_vm_cur_safe(vm)) { \
        vm->mode = mt_pfx(VM_ERR); \
    } else { \
        mt_vm_ret(vm); \
    }

mt_var mt_vm_run(mt_vm* const vm);

void mt_vm_debug_stack_print(const mt_vm* const vm);

#endif
