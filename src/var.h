
#ifndef MT_VAR_H
#define MT_VAR_H

#include <stdbool.h>
#include "mod.h"

typedef struct _mt_var {
    enum {
        MT_NULL,
        MT_BOOl,
        MT_CHAR,
        MT_INT,
        MT_FLOAT,
        MT_FN,
        // @TODO colletions arrays modules hashs
    } type;
    union {
        bool mt_bool;
        char mt_char;
        int64_t mt_int;
        double mt_float;
        uint8_t* mt_fn;
    } data;
} mt_var;

#endif
