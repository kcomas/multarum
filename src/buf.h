
#ifndef MT_BUF_H
#define MT_BUF_H

#include <stdlib.h>
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

typedef struct {
    size_t buf_pos;
    mt_buf* buf;
} mt_buf_iter;

#endif
