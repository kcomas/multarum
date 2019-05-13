
#pragma once

#include <errno.h>

typedef struct {
    enum {
        ERR_OK,
        ERR_SYSTEM
    } type;
    int no;
} err;

#define err_init(e) e.type = ERR_OK

#define err_system(e) \
    e->type = ERR_SYSTEM; \
    e->no = errno;
