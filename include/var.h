
#ifndef VAR_H
#define VAR_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "utf8.h"
#include "str.h"
#include "vec.h"
#include "dict.h"
#include "file.h"

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
        double b; // float
        utf8 c; // char
        str s;
        int fd; // file descriptor
        vec v;
        dict d;
    } value;
} var;

#define var_err_c(c_str) (var) { .type = -9, .ref_count = 1, .value = { .s = str_from_c(c_str) } }

void var_free(var v);

#endif
