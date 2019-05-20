
#pragma once

#include <stdlib.h>
#include "var.h"
#include "str.h"
#include "err.h"

typedef struct _hash_bucket {
    struct _hash_bucket *next;
    str *key;
    var value;
} hash_bucket;

typedef struct _hash {
    size_t size, used;
    hash_bucket *buckets[];
} hash;

hash *hash_init(size_t size);

void hash_free(hash *const h);

#define HASH_GROW_MULTIPLIER 2

hash *hash_clone(const hash *const  h, size_t size);

void hash_insert(hash **const h, str *const key, var value);

var hash_get(const hash *const h, const str *const key, err *const e);
