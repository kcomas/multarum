
#ifndef VEC_H
#define VEC_H

#include "var.h"

typedef struct _vec {
    size_t len, size;
    var data[];
} *vec;

vec vec_init(size_t size);

void vec_free(vec v);

bool vec_push(vec* v, var* err, var value);

bool vec_concat(vec* v, var* err, vec x, vec y);

bool vec_pop(vec v, var* err, var value);

bool vec_insert(vec* v, var* err, size_t idx, var value);

bool vec_remove(vec v, var* err, size_t idx);

#endif
