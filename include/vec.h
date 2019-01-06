
#ifndef VEC_H
#define VEC_H

#include "var.h"

typedef struct _vec {
    size_t len, size;
    var data[];
} *vec;

vec vec_init(size_t size);

void vec_free(vec v);

void vec_push(vec* v, var value);

void vec_concat(vec* x, vec y);

vec vec_copy(vec v);

void vec_reverse(vec v);

bool vec_pop(vec v, var* err, var* value);

bool vec_insert(vec* v, var* err, size_t idx, var value);

bool vec_remove(vec v, var* err, size_t idx, var* value);

bool vec_cmp(vec x, vec y);

void vec_print(const vec v);

#endif
