
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "str.h"

typedef struct _hash hash;

typedef struct _var {
    enum {
        VAR_NULL,
        VAR_BOOL,
        VAR_INT,
        VAR_FLOAT,
        VAR_FILE,
        VAR_STR,
        VAR_HASH
    } type;
    union {
        bool b;
        int64_t i;
        double f;
        int fd;
        str *s;
        hash *h;
    } value;
} var;

#define var_null (var) { .type = VAR_NULL }

#define var_str(str) (var) { .type = VAR_STR, .value = { .s = str } }

#define var_hash(hash) (var) { .type = VAR_HASH, .value = { .h = hash } }

#include "hash.h"

void var_free(var v);

var var_copy(var v);
