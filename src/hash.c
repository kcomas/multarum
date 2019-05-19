
#include "hash.h"

hash hash_init(size_t size) {
    hash h = calloc(1, sizeof(struct _hash) + sizeof(hash_bucket) * size);
    h->size = size;
    h->used = 0;
    return h;
}

void hash_free(hash h) {
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] == NULL) continue;
        hash_bucket b = h->buckets[i];
        while (b != NULL) {
            hash_bucket tmp = b->next;
            b = b->next;
            str_free(tmp->key);
            var_free(tmp->value);
            free(tmp);
        }
    }
    free(h);
}

static inline hash_bucket hash_bucket_clone(hash_bucket b) {
    hash_bucket clone = calloc(1, sizeof(struct _hash_bucket));
    hash_bucket tmp = clone;
    while (b != NULL) {
       tmp->value = b->value;
       tmp->key = b->key;
       tmp = tmp->next;
       b = b->next;
    }
    return clone;
}

hash hash_clone(hash h, size_t size) {
    hash clone = hash_init(size);
    clone->size = size;
    clone->used = h->used;
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] != 0) continue;
        clone->buckets[i] = hash_bucket_clone(h->buckets[i]);
    }
    return clone;
}

static inline size_t hash_hash(str s) {
    size_t hash = 5381;
    char* data = s->data;
    while (*data) hash = ((hash << 5) + hash) + *data;
    return hash;
}

void hash_insert(hash *h, str key, var value) {
    if ((*h)->used == (*h)->size) {
        hash clone = hash_clone(*h, (*h)->size * HASH_GROW_MULTIPLIER);
        hash_free(*h);
        *h = clone;
    }
    size_t idx = hash_hash(key) % (*h)->size;
    hash_bucket b = calloc(1, sizeof(struct _hash_bucket));
    b->key = key;
    b->value = value;
    hash_bucket pos = (*h)->buckets[idx];
    while (pos != NULL) {
        if (pos->next == NULL) {
            pos->next = b;
            break;
        }
        pos = pos->next;
    }
}
