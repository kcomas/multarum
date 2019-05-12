
#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct _var {
    enum {
        VAR_BOOL,
        VAR_INT,
        VAR_FLOAT,
        VAR_FILE
    } type;
    union {
        bool b;
        int64_t i;
        double f;
        int file;
    } value;
} *var;
