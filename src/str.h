
#ifndef MT_STR_H
#define MT_STR_H

#include "buf.h"

typedef struct _mt_str_link {
    mt_buf* data;
    struct _mt_str_link* next;
} mt_str_link;

typedef struct {
    size_t len, _size;
    mt_str_link* str_head;
    mt_str_link* str_tail;
} mt_str;

mt_str* mt_str_init(mt_buf* const buf);

#endif
