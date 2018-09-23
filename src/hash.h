
#ifndef MT_HASH_H
#define MT_HASH_H

#include "var.h"

typedef struct _mt_hash_node {
    int32_t key;
    uint8_t* name;
    struct _mt_hash_node* next;
    mt_var value;
} mt_hash_node;

typedef struct {
    uint32_t ref_count;
    size_t len, _bsize; // len can be larger then _bsize
    mt_hash_node* buckets;
} mt_hash;

mt_hash* mt_hash_init(size_t num_buckets);

#endif
