
#include "hash.h"

mt_hash* mt_hash_init(size_t num_buckets) {
    mt_hash* h = (mt_hash*) malloc(sizeof(mt_hash));
    h->ref_count = 0;
    h->len = 0;
    h->_bsize = num_buckets;
    h->buckets = (mt_hash_node*) malloc(sizeof(mt_hash_node) * num_buckets);
    return h;
}
