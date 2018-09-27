
#include "hash.h"

mt_hash* mt_hash_init(size_t num_buckets) {
    mt_hash* h = (mt_hash*) malloc(sizeof(mt_hash));
    h->ref_count = 0;
    h->len = 0;
    h->_bsize = num_buckets;
    h->buckets = (mt_hash_node*) malloc(sizeof(mt_hash_node) * num_buckets);
    return h;
}

mt_var mt_hash_insert(mt_hash* const hash, const mt_buf* const name, mt_var value) {
    if (name->len > MT_HASH_MAX_NAME_SIZE) {
        return mt_var_err(mt_err_hash_key_len());
    }
    size_t hashd = 5381;
    for (size_t i = 0; i < name->len; i++) {
        hashd = ((hashd << 5) + hashd) + (name->data[i]);
    }
}
