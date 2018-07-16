
#ifndef MT_VM
#define MT_VM

#include "multarum.h"

#ifndef MT_DEFAULT_STACK_SIZE
    #define MT_DEFAULT_STACK_SIZE 2000
#endif

#ifndef MT_DEFAULT_FRAME_SIZE
    #define MT_DEFAULT_FRAME_SIZE 200
#endif

typedef struct {
    size_t rbp;
    uint8_t* rip;
    uint8_t* code;
} mt_frame;

typedef struct {
    size_t s_len, f_len;
    mt_var* stack;
    mt_frame* rsp;
} mt_vm;

#define mt_vm_cur_byte(vm) vm->rsp[vm->f_len - 1].rip

#define mt_vm_cur_code(vm) vm->rsp[vm->f_len - 1].code

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

void mt_vm_init(mt_vm* const vm, const mt_mod* const mod);

void mt_vm_free(mt_vm* const vm);

void mt_vm_dec_stack(mt_vm* const vm);

mt_var mt_vm_run(mt_vm* const vm);

#endif
