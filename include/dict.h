
#ifndef DICT_H
#define DICT_H

#include "var.h"

typedef struct _dict_node {
    struct _dict_node* next;
    var data;
} dict_node;

typedef struct _dict {
    size_t used, size; // used can be bigger then size
    dict_node buckets[];
} *dict;

dict dict_init(size_t size);

void dict_free(dict d);

dict dict_concat(dict x, dict y);

void dict_insert(dict* d, var key, var value);

bool dict_remove(dict d, var* err, var key);

void dict_print(const dict d);

#endif
