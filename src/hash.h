
#ifndef MT_HASH_H
#define MT_HASH_H

#include "var.h"

#define MT_HASH_FULL_RESIZE 0.70

typedef struct _mt_hash_node {
   int32_t key;
   uint8_t* name;
   struct _mt_hash_node* next;
   mt_var value;
} mt_hash_node;

mt_hash_node* mt_hash_init(size_t buckets);

#endif
