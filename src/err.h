
#ifndef MT_ERR
#define MT_ERR

#include <stdint.h>
#include "common.h"

typedef enum {
    mt_pfx(ERR_OK)
} mt_err_type;

typedef struct {
    bool in_fn;
    mt_err_type type;
    uint8_t fn_idx;
    char* msg;
    mt_mod* mod;
} mt_err;

#endif
