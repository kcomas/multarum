
#ifndef MT_HASH_H
#define MT_HASH_H

#include "var.h"

typedef struct _mt_hash_node {
    size_t hashd;
    mt_buf* name;
    struct _mt_hash_node* next;
    mt_var value;
} mt_hash_node;

typedef struct _mt_hash {
    uint32_t ref_count;
    size_t len, _bsize; // len can be larger then _bsize
    mt_hash_node** buckets;
} mt_hash;

mt_hash* mt_hash_init(size_t num_buckets);

void mt_hash_free(mt_hash* const hash);

#ifndef MT_HASH_MAX_NAME_SIZE // in bytes not chars
#   define MT_HASH_MAX_NAME_SIZE 100
#endif

mt_var mt_hash_insert(mt_hash* const hash, const mt_buf* const name, mt_var value);

mt_var mt_hash_get(mt_hash* const hash, const mt_buf* const name);

void mt_hash_debug_print(const mt_hash* const hash, uint32_t indent);

#endif
