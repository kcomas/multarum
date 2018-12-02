
#ifndef MT_STR_H
#define MT_STR_H

#include "buf.h"

typedef struct _mt_str_link {
    mt_buf* data;
    struct _mt_str_link* next;
} mt_str_link;

typedef struct {
    uint32_t ref_count;
    size_t len, _chunks;
    mt_str_link* head;
    mt_str_link* tail;
} mt_str;

mt_str* mt_str_init(mt_buf* const buf);

void mt_str_free(mt_str* const str);

void mt_str_debug_print(const mt_str* const str);

#endif
