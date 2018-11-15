
#ifndef MT_VAR_H
#define MT_VAR_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "char.h"
#include "mod.h"
#include "buf.h"
#include "err.h"

typedef struct _mt_mod mt_mod;
typedef struct _mt_err mt_err;
typedef struct _mt_hash mt_hash;

typedef enum {
    MT_TYPES
    // @TODO colletions arrays
} mt_var_type;

typedef struct _mt_var {
    mt_var_type type;
    union {
        uint8_t fn_idx;
    } meta;
    union {
        bool mt_bool;
        mt_char mt_char;
        int mt_file;
        int64_t mt_int;
        double mt_float;
        mt_mod* mt_mod;
        mt_err* mt_err;
        mt_buf* mt_buf;
        mt_hash* mt_hash;
    } data;
} mt_var;

#include "hash.h"

#define mt_var_null() (mt_var) { .type = mt_pfx(NULL) }

#define mt_var_bool(value) (mt_var) { .type = mt_pfx(BOOL), .data = { .mt_bool = value } }

#define mt_var_char(value) (mt_var) { .type = mt_pfx(CHAR), .data = { .mt_char = value } }

#define mt_var_int(value) (mt_var) { .type = mt_pfx(INT), .data = { .mt_int = value } }

#define mt_var_float(value) (mt_var) { .type = mt_pfx(FLOAT), .data = { .mt_float = value } }

#define mt_var_mod(value) (mt_var) { .type = mt_pfx(MODULE), .data = { .mt_mod = value } }

#define mt_var_fn(value, i) (mt_var) { .type = mt_pfx(FN), .meta = { .fn_idx = i }, .data = { .mt_mod = value } }

#define mt_var_file(value) (mt_var) { .type = mt_pfx(MFILE), .data = { .mt_file = value } }

#define mt_var_err(value) (mt_var) { .type = mt_pfx(ERROR), .data = { .mt_err = value } }

#define mt_var_buf(value) (mt_var) { .type = mt_pfx(BUFFER), .data = { .mt_buf = value } }

#define mt_var_is_null(var) (var.type == mt_pfx(NULL))

#define mt_var_is_bool(var) (var.type == mt_pfx(BOOL))

#define mt_var_bool_value(var) var.data.mt_bool

#define mt_var_is_err(var) (var.type == mt_pfx(ERROR))

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var);

bool mt_var_as_bool(const mt_var* const var);

void mt_mod_free(mt_mod* const mod);
void mt_hash_free(mt_hash* const hash);
void mt_err_free(mt_err* const err);

inline void mt_var_free(mt_var var) {
    switch (var.type) {
        case mt_pfx(MODULE):
        case mt_pfx(FN):
            mt_mod_free(var.data.mt_mod);
            break;
        case mt_pfx(ERROR):
            mt_err_free(var.data.mt_err);
            break;
        case mt_pfx(BUFFER):
            mt_buf_free(var.data.mt_buf);
            break;
        case mt_pfx(HASH):
            mt_hash_free(var.data.mt_hash);
        default:
            break;
    }
}

void mt_var_debug_print(mt_var var);

#endif
