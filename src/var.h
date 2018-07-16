
#ifndef MT_VAR_H
#define MT_VAR_H

#include "multarum.h"

typedef struct _mt_var {
    enum {
        MT_NULL,
        MT_BOOL,
        MT_CHAR,
        MT_INT,
        MT_FLOAT,
        // MT_FN,
        // MT_MOD
        // @TODO colletions arrays modules hashs
    } type;
    int16_t ref_count;
    uint8_t fn_idx;
    union {
        bool mt_bool;
        char mt_char;
        int64_t mt_int;
        double mt_float;
    } data;
} mt_var;

#define mt_var_null (mt_var) { .type = MT_NULL, .ref_count = -1 }

#define mt_var_bool(value) (mt_var) { .type = MT_BOOL, .ref_count = -1, .data = { .mt_bool = value } }

#define mt_var_char(value) (mt_var) { .type = MT_CHAR, .ref_count = -1, .data = { .mt_char = value } }

#define mt_var_int(value) (mt_var) { .type = MT_INT, .ref_count = -1, .data = { .mt_int = value } }

#define mt_var_float(value) (mt_var) { .type = MT_FLOAT, .ref_count = -1, .data = { .mt_float = value } }

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var);

#endif
