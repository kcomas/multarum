
#ifndef MT_ERR
#define MT_ERR

#include "multarum.h"

typedef enum {
    mt_pfx(ERR_OK)
} mt_err_type;

typedef struct {
    mt_err_type type;
    char* msg;
} mt_err;

#endif
