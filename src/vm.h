
#ifndef MT_VM
#define MT_VM

#include <string.h>
#include "var.h"
#include "op.h"

#ifndef MT_DEFAULT_STACK_SIZE
    #define MT_DEFAULT_STACK_SIZE 2000
#endif

#ifndef MT_DEFAULT_FRAME_SIZE
    #define MT_DEFAULT_FRAME_SIZE 200
#endif

typedef struct {
    size_t s_len, r_len;
    mt_var* stack;
    uint8_t** rip;
} mt_vm;

#define mt_vm_cur_byte(vm) vm->rip[vm->r_len - 1]

#define mt_vm_cur_stack(vm) vm->stack[vm->s_len - 1]

#define mt_vm_prev_stack(vm) vm->stack[vm->s_len - 2]

#define mt_vm_dec_stack(vm) vm->s_len--;

#define mt_vm_push(vm, var) vm->stack[vm->s_len++] = var;

#define mt_vm_get_bytes(vm, dest, size) \
    memcpy(dest, mt_vm_cur_byte(vm), size); \
    mt_vm_cur_byte(vm) += size

void mt_vm_init(mt_vm* const vm, const mt_mod* const mod);

void mt_vm_free(mt_vm* const vm);

mt_var mt_vm_run(mt_vm* const vm);

#endif
