
#ifndef MT_VAR_H
#define MT_VAR_H

#include <stdbool.h>
#include "mod.h"

typedef struct {
    enum {
        MT_NULL,
        MT_BOOL,
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

#define mt_var_null (mt_var) { .type = MT_NULL }

#define mt_var_bool(value) (mt_var) { .type = MT_BOOL, .data = { .mt_bool = value } }

#define mt_var_char(value) (mt_var) { .type = MT_CHAR, .data = { .mt_char = value } }

#define mt_var_int(value) (mt_var) { .type = MT_INT, .data = { .mt_int = value } }

#define mt_var_float(value) (mt_var) { .type = MT_FLOAT, .data = { .mt_float = value } }

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var);

#endif
