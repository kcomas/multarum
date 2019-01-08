
#ifndef DICT_H
#define DICT_H

#include "var.h"

typedef struct _dict_node {
    str key;
    struct _dict_node* next;
    var value;
} *dict_node;

typedef struct _dict {
    uint16_t ref_count;
    size_t used, size; // used can be bigger then size
    dict_node buckets[];
} *dict;

dict dict_init(size_t size);

void dict_free(dict d);

bool dict_get(dict d, var* err, str key, var* value);

void dict_concat(dict* x, dict y);

void dict_insert(dict* d, str key, var value);

bool dict_remove(dict d, var* err, str key, var* value);

void dict_print(const dict d);

#endif
