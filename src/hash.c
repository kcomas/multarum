
#include "hash.h"

mt_hash* mt_hash_init(size_t num_buckets) {
    mt_hash* h = (mt_hash*) malloc(sizeof(mt_hash));
    h->ref_count = 1;
    h->len = 0;
    h->_bsize = num_buckets;
    h->buckets = (mt_hash_node**) malloc(sizeof(mt_hash_node*) * num_buckets);
    for (size_t i = 0; i < h->_bsize; i++) {
        h->buckets[i] = NULL;
    }
    return h;
}

void mt_hash_free(mt_hash* const hash) {
    for (size_t i = 0; i < hash->_bsize; i++) {
        if (hash->buckets[i] == NULL) {
            continue;
        }
        mt_hash_node* next = hash->buckets[i]->next;
        while (next != NULL) {
            mt_hash_node* tmp = next;
            next = next->next;
            free(tmp);
        }
        free(hash->buckets[i]);
    }
    free(hash->buckets);
}

static mt_hash_node* mt_hash_create_node(size_t hashd, const mt_buf* const name, mt_var value) {
    mt_hash_node* node = (mt_hash_node*) malloc(sizeof(mt_hash_node));
    node->hashd = hashd;
    node->name = mt_buf_clone(name);
    node->next = NULL;
    node->value = value;
    return node;
}

static size_t mt_hash_hash(const mt_buf* const name) {
    size_t hashd = 5381;
    for (size_t i = 0; i < name->len; i++) {
        hashd = ((hashd << 5) + hashd) + (name->data[i]);
    }
    return hashd;
}

mt_var mt_hash_insert(mt_hash* const hash, const mt_buf* const name, mt_var value) {
    if (name->len > MT_HASH_MAX_NAME_SIZE) {
        return mt_var_err(mt_err_hash_key_len());
    }
    size_t hashd = mt_hash_hash(name);
    size_t pos = hashd % hash->_bsize;
    if (hash->buckets[pos] == NULL) {
        mt_hash_node* node = mt_hash_create_node(hashd, name, value);
        hash->buckets[pos] = node;
    } else {
        mt_hash_node* to_ins = hash->buckets[pos];
        for (;;) {
            if (mt_buf_cmp(name, to_ins->name)) {
                to_ins->value = value;
                break;
            } else if (to_ins->next == NULL) {
                mt_hash_node* node = mt_hash_create_node(hashd, name, value);
                to_ins->next = node;
                break;
            }
            to_ins = to_ins->next;
        }
    }
    hash->len++;
    return mt_var_bool(true);
}

mt_var mt_hash_get(mt_hash* const hash, const mt_buf* const name) {
    size_t hashd = mt_hash_hash(name);
    size_t pos = hashd % hash->_bsize;
    if (hash->buckets[pos] == NULL) {
        return mt_var_null();
    }
    mt_hash_node* to_get = hash->buckets[pos];
    while (to_get != NULL) {
        if (mt_buf_cmp(name, to_get->name)) {
            return to_get->value;
        }
        to_get = to_get->next;
    }
    return mt_var_null();
}

void mt_hash_debug_print(const mt_hash* const hash) {
    for (size_t i = 0; i < hash->_bsize; i++) {
        if (hash->buckets[i] == NULL) {
            continue;
        }
        printf("%lu: ", i);
        mt_buf_debug_print(hash->buckets[i]->name);
        printf(" - ");
        mt_var_debug_print(hash->buckets[i]->value);
        mt_hash_node* next = hash->buckets[i]->next;
        size_t sub = 0;
        while (next != NULL) {
            printf("\n\t%lu.%lu: ", i, sub++);
            mt_buf_debug_print(hash->buckets[i]->name);
            printf(" - ");
            mt_var_debug_print(hash->buckets[i]->value);
            next = next->next;
        }
        printf("\n");
    }
}
