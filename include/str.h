
#ifndef STR_H
#define STR_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "utf8.h"

typedef struct _str {
    uint16_t ref_count;
    size_t len, size;
    uint8_t data[];
} *str;

str str_init(size_t size);

void str_free(str s);

void str_push(str* s, utf8 c);

void str_concat(str* x, str y);

str str_copy(str s);

str str_from_c(const char* const c_str);

char* str_to_c(const str s);

bool str_cmp(const str x, const str y);

void str_print(const str s);

/**
 * Strict utf8 iter
 * Check first byte for continuations
 * From highest to lowest
 * 0xxxxxxxx 127 value ascii compat
 * 10xxxxxxx error
 * 110xxxxxx 1 continuation 128 to 2047
 * 1110xxxxx 2 continuation 2048 to 65535 excluding 55296 to 57343
 * 11110xxxx 3 65536 to 1114111
 * 11111xxxx error
 */

typedef struct {
    size_t idx;
    str s;
} str_iter;

void str_iter_init(str_iter* const si, str s);

bool str_iter_peek(str_iter* const si, utf8* c);

bool str_iter_next(str_iter* const si, utf8* c);

#endif
