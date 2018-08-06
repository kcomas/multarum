
#ifndef MT_VAR
#define MT_VAR

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "mod.h"
#include "err.h"

typedef struct _mt_mod mt_mod;
typedef struct _mt_err mt_err;

typedef struct _mt_var {
    enum {
        mt_pfx(NULL),
        mt_pfx(BOOL),
        mt_pfx(CHAR),
        mt_pfx(INT),
        mt_pfx(FLOAT),
        mt_pfx(MODULE),
        mt_pfx(FN),
        mt_pfx(MFILE),
        mt_pfx(ERROR)
        // @TODO colletions arrays modules hashs
    } type;
    uint8_t fn_idx;
    union {
        bool mt_bool;
        char mt_char;
        int32_t mt_file;
        int64_t mt_int;
        double mt_float;
        mt_mod* mt_mod;
        mt_err* mt_err;
    } data;
} mt_var;

#define mt_var_null (mt_var) { .type = mt_pfx(NULL) }

#define mt_var_bool(value) (mt_var) { .type = mt_pfx(BOOL), .data = { .mt_bool = value } }

#define mt_var_char(value) (mt_var) { .type = mt_pfx(CHAR), .data = { .mt_char = value } }

#define mt_var_int(value) (mt_var) { .type = mt_pfx(INT), .data = { .mt_int = value } }

#define mt_var_float(value) (mt_var) { .type = mt_pfx(FLOAT), .data = { .mt_float = value } }

#define mt_var_mod(value) (mt_var) { .type = mt_pfx(MODULE), .data = { .mt_mod = value } }

#define mt_var_fn(value, i) (mt_var) { .type = mt_pfx(FN), .fn_idx = i, .data = { .mt_mod = value } }

#define mt_var_file(value) (mt_var) { .type = mt_pfx(MFILE), .data = { .mt_file = value } }

#define mt_var_err(value) (mt_var) { .type = mt_pfx(ERROR), .data = { .mt_err = value } }

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var);

bool mt_var_as_bool(const mt_var* const var);

void mt_var_debug_print(const mt_var* const var);

#endif
