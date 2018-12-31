
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

typedef struct _str *str;
typedef struct _vec *vec;
typedef struct _dict *dict;

typedef struct {
    int8_t type; // positive denotes vector of type, negative is atom
    union {
        uint8_t fn_idx; // function index in mod
    } meta;
    uint16_t ref_count;
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

#endif
