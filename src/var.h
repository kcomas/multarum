
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct _hash *hash;

typedef struct _var {
    enum {
        VAR_BOOL,
        VAR_INT,
        VAR_FLOAT,
        VAR_FILE,
        VAR_HASH
    } type;
    union {
        bool b;
        int64_t i;
        double f;
        int fd;
        hash h;
    } value;
} var;

#include "hash.h"

void var_free(var v);
