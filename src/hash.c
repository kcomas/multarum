
#include "hash.h"

mt_hash_node* mt_hash_init(size_t buckets) {
    return (mt_hash_node*) malloc(sizeof(mt_hash_node) * buckets);
}
