
#include "hash.h"

hash *hash_init(size_t size) {
    hash *h = calloc(1, sizeof(hash) + sizeof(hash_bucket) * size);
    h->size = size;
    h->used = 0;
    return h;
}

void hash_free(hash *const h) {
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] == NULL) continue;
        hash_bucket *b = h->buckets[i];
        while (b != NULL) {
            hash_bucket *tmp = b->next;
            b = b->next;
            str_free(tmp->key);
            var_free(tmp->value);
            free(tmp);
        }
    }
    free(h);
}

static inline hash_bucket *hash_bucket_clone(const hash_bucket *b) {
    hash_bucket *clone = calloc(1, sizeof(struct _hash_bucket));
    hash_bucket *tmp = clone;
    while (b != NULL) {
        // values must be cloned
        tmp->value = b->value;
        tmp->key = str_clone(b->key, b->key->size - 1);
        tmp = tmp->next;
        b = b->next;
    }
    return clone;
}

hash *hash_clone(const hash *const h, size_t size) {
    hash *clone = hash_init(size);
    clone->size = size;
    clone->used = h->used;
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] != NULL) continue;
        clone->buckets[i] = hash_bucket_clone(h->buckets[i]);
    }
    return clone;
}

static inline size_t hash_hash(const str const* s) {
    size_t hash = 5381;
    for (size_t i = 0; i < s->len; i++) hash = ((hash << 5) + hash) + s->data[i];
    return hash;
}

void hash_insert(hash **const h, str *const key, var value) {
    if ((*h)->used == (*h)->size) {
        hash *clone = hash_clone(*h, (*h)->size * HASH_GROW_MULTIPLIER);
        hash_free(*h);
        *h = clone;
    }
    size_t idx = hash_hash(key) % (*h)->size;
    hash_bucket *b = calloc(1, sizeof(struct _hash_bucket));
    // does not clone values
    b->key = key;
    b->value = value; // TODO var clone
    hash_bucket *pos = (*h)->buckets[idx];
    while (pos != NULL) {
        if (pos->next == NULL) {
            pos->next = b;
            break;
        }
        pos = pos->next;
    }
}

var hash_get(const hash *const h, const str *const key, err *const e) {
    size_t idx = hash_hash(key) % h->size;
    if (h->buckets[idx] == NULL) {
        err_basic(e, ERR_KEY_NOT_IN_HASH);
        return var_null;
    }
    hash_bucket *b = h->buckets[idx];
    while (b != NULL) {
        if (strcmp(key->data, b->key->data) == 0) return b->value;
        b = b->next;
    }
    err_basic(e, ERR_KEY_NOT_IN_HASH);
    return var_null;
}
