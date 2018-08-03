
#ifndef MT_ERR
#define MT_ERR

#include <stdint.h>
#include "common.h"
#include "vm.h"

typedef enum {
    mt_pfx(ERR_OK)
} mt_err_type;

#ifndef MT_ERR_STACK_COPY
#   define MT_ERR_STACK_COPY 10
#endif

typedef struct {
    uint32_t ref_count;
    int32_t no; // errno
    size_t f_len;
    mt_frame* rsp;
    char* msg;
} mt_err;

#endif
