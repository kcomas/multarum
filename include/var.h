
#ifndef VAR_H
#define VAR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "utf8.h"
#include "str.h"

typedef struct _str *str;
typedef struct _vec *vec;
typedef struct _dict *dict;
typedef struct _vfd *vfd;

typedef struct {
    int8_t type; // positive denotes vector of type, negative is atom
    union {
        uint8_t fn_idx; // function index in mod
    } meta;
    union {
        bool b; // bool
        int64_t i; // int
        double f; // float
        utf8 c; // char
        str s;
        vfd fd; // file descriptor
        vec v;
        dict d;
    } value;
} var;

#include "vec.h"
#include "dict.h"
#include "file.h"

#define var_bool(VALUE) (var) { .type = -1, .value = { .b = VALUE } }

#define var_int(VALUE) (var) { .type = -2, .value = { .i = VALUE } }

#define var_float(VALUE) (var) { .type = -3, .value = { .f = VALUE } }

#define var_utf8(VALUE) (var) { .type = -4, .value = { .c = VALUE } }

#define var_str(VALUE) (var) { .type = 4, .value = { .s = VALUE } }

#define var_vec(VALUE) (var) { .type = 0, .value = { .v = VALUE } }

#define var_fd(VALUE) (var) { .type = -5, .value = { .fd = VALUE } }

#define var_err_c(c_str) (var) { .type = -9, .value = { .s = str_from_c(c_str) } }

void var_free(var v);

void var_inc_ref(var v);

bool var_cmp(var x, var y);

void var_print(const var v);

#endif
