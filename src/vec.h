
#ifndef MT_VEC
#define MT_VEC

#include "var.h"

typedef enum {
    MT_TYPES,
    mt_pfx(MIXED)
} mt_vec_type;

typedef struct {
    mt_vec_type type;
    size_t len, size;
    mt_var** items;
} mt_vec;

#endif
