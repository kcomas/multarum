
#ifndef MT_BUF_H
#define MT_BUF_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "char.h"

typedef struct {
    size_t len, _size;
    uint8_t* data;
} mt_buf;

mt_buf* mt_buf_init(size_t _size);

mt_buf* mt_buf_from_c_str(const char* const str);

void mt_buf_free(mt_buf* const buf);

void mt_buf_zero(mt_buf* const buf);

typedef struct {
    size_t buf_pos;
    const mt_buf* buf;
} mt_buf_iter;

#define mt_buf_space_free(buf) buf->_size - buf->len

void mt_buf_iter_init(const mt_buf* const buf, mt_buf_iter* const iter);

bool mt_buf_iter_next(mt_buf_iter* const iter, mt_char* const c);

bool mt_buf_iter_peek(mt_buf_iter* const iter, mt_char* const c);

bool mt_buf_push_byte(mt_buf* const buf, uint8_t b);

bool mt_buf_push_char(mt_buf* const buf, mt_char c);

mt_buf* mt_buf_clone(const mt_buf* const buf);

bool mt_buf_cmp(const mt_buf* const a, const mt_buf* const b);

bool mt_buf_write(mt_buf* restrict buf, const void* restrict data, size_t size);

void mt_buf_debug_print(const mt_buf* const buf);

#endif
