
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

typedef struct {
    int8_t type; // positive denotes vector of type, negative is atom
    union {
        uint8_t fn_idx; // function index in mod
    } meta;
    int16_t ref_count; // negative denotes non ref counted type
    union {
        bool b; // bool
        int64_t i; // int
        double f; // float
        utf8 c; // char
        str s;
        int fd; // file descriptor
        vec v;
        dict d;
    } value;
} var;

#include "vec.h"
#include "dict.h"
#include "file.h"

#define var_noref(TYPE, VALUE) (var) { .type = TYPE, .ref_count = -1, .value = VALUE }

#define var_bool(VALUE) var_noref(-1, { .b = VALUE })

#define var_int(VALUE) var_noref(-2, { .i = VALUE })

#define var_float(VALUE) var_noref(-3, { .f = VALUE })

#define var_utf8(VALUE) var_noref(-4, { .c = VALUE })

#define var_err_c(c_str) (var) { .type = -9, .ref_count = 1, .value = { .s = str_from_c(c_str) } }

void var_free(var* const v);

bool var_cmp(var x, var y);

void var_print(const var v);

#endif
