
#ifndef MT_VAR_H
#define MT_VAR_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    enum {
        MT_NULL,
        MT_BOOl,
        MT_CHAR,
        MT_INT,
        MT_FLOAT,
        MT_FN
        // @TODO colletions arrays mods hash
    } type;
    union {
        bool mt_bool;
        char mt_char;
        int64_t mt_int;
        double mt_float;
        void* mt_fn;
    } data;
} mt_var;

#endif
