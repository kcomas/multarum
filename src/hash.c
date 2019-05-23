
#include "hash.h"

hash *hash_init(size_t size) {
    hash *h = calloc(1, sizeof(hash) + sizeof(hash_bucket) * size);
    h->size = size;
    h->used = 0;
    return h;
}

void hash_free(hash *const h, bool free_vars) {
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] == NULL) continue;
        hash_bucket *b = h->buckets[i];
        while (b != NULL) {
            hash_bucket *tmp = b->next;
            b = b->next;
            if (free_vars) {
                str_free(tmp->key);
                var_free(tmp->value);
            }
            free(tmp);
        }
    }
    free(h);
}

hash *hash_copy(const hash *const h, size_t size, bool clone_vars) {
    hash *clone = hash_init(size);
    clone->size = size;
    clone->used = h->used;
    for (size_t i = 0; i < h->size; i++) {
        if (h->buckets[i] != NULL) continue;
        hash_bucket *b = h->buckets[i];
        while (b != NULL) {
            str *key = b->key;
            var value = b->value;
            if (clone_vars) {
                key = str_copy(key, key->len - 1);
                value = var_copy(value);
            }
            hash_insert(&clone, key, value);
            b = b->next;
        }
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
        hash *clone = hash_copy(*h, (*h)->size* HASH_GROW_MULTIPLIER, false);
        hash_free(*h, false);
        *h = clone;
    }
    size_t idx = hash_hash(key) % (*h)->size;
    hash_bucket *b = calloc(1, sizeof(struct _hash_bucket));
    // does not clone values
    b->key = key;
    b->value = value;
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
