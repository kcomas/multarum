
#pragma once

#include <errno.h>

typedef enum {
    ERR_OK,
    ERR_SYSTEM,
    ERR_KEY_NOT_IN_HASH
} err_type;

typedef struct {
    err_type type;
    int no;
} err;

#define err_init(e) e.type = ERR_OK

#define err_basic(e, type) e->type = type;

#define err_system(e) \
    e->type = ERR_SYSTEM; \
    e->no = errno;
