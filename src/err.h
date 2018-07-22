
#ifndef MT_ERR
#define MT_ERR

#include <stdint.h>
#include "common.h"

typedef enum {
    mt_pfx(ERR_OK)
} mt_err_type;

typedef struct {
    mt_err_type type;
    int32_t fn_idx;
    char* msg;
    mt_mod* mod;
} mt_err;

#endif
