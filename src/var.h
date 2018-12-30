
#ifndef VAR_H
#define VAR_H

#include <stdbool.h>
#include <stdint.h>
#include "vec.h"
#include "dict.h"

typedef struct _vec *vec;
typedef struct _dict *dict;

typedef struct {
    int8_t type; // positive denotes vector of type, negative is atom
    uint16_t ref_count;
    union {
        uint8_t fn_idx; // function index in mod
    } meta;
    union {
        bool b; // bool
        int64_t i; // int
        double b; // float
        char c[4]; // char
        int fd; // file descriptor
        vec v;
        dict d;
    } value;
} var;

#endif
