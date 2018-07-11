
#ifndef MT_VM
#define MT_VM

#include "var.h"

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

void mt_vm_init(mt_vm* const vm);

void mt_vm_free(mt_vm* const vm);

#endif
