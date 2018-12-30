
#ifndef VEC_H
#define VEC_H

#include "var.h"

typedef struct _vec {
    size_t len, size;
    var data[];
} *vec;

vec vec_init(size_t size);

void vec_free(vec v);

bool vec_insert(vec v, var value);

bool vec_pop(vec v, var* err, var value);

#endif
