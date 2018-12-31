
#ifndef STR_H
#define STR_H

#include "var.h"

typedef struct _str {
    size_t len, size;
    uint8_t data[];
} *str;

str str_init(size_t size);

void str_free(str s);

bool str_push(str* s, var* err, utf8 c);

bool str_concat(str* s, var* err, str x, str y);

bool str_from_c(str* s, var* err, char* c_str);

typedef struct {
    size_t idx;
    str s;
} str_iter;

bool str_iter_init(str_iter* si, var* err);

bool str_iter_peek(str_iter* si, utf8* c);

bool str_iter_next(str_iter* si, utf8* c);

#endif
