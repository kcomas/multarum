
#ifndef DICT_H
#define DICT_H

#include "var.h"

typedef struct _dict_node {
    struct _dict_node* next;
    var data;
} dict_node;

typedef struct _dict {
    size_t used, size;
    dict_node buckets[];
} *dict;

dict dict_init(size_t size);

void dict_free(dict d);

bool dict_insert(dict d, var* err, var key, var value);

#endif
