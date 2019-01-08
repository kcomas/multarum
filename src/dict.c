
#include "dict.h"

dict dict_init(size_t size) {
    dict d = (dict) malloc(sizeof(struct _dict) + size * sizeof(dict_node));
    d->ref_count = 1;
    d->used = 0;
    d->size = size;
    for (size_t i = 0; i < d->size; i++) d->buckets[i] = NULL;
    return d;
}

static inline dict_node dict_node_init(str key, var value) {
    key->ref_count++;
    var_inc_ref(value);
    dict_node n = (dict_node) malloc(sizeof(struct _dict_node));
    n->key = key;
    n->value = value;
    n->next = NULL;
    return n;
}

static inline void dict_node_free(dict_node n) {
    while (n != NULL) {
        dict_node tmp = n;
        n = n->next;
        str_free(tmp->key);
        var_free(tmp->value);
        free(tmp);
    }
}

void dict_free(dict d) {
    for (size_t i = 0; i < d->size; i++) dict_node_free(d->buckets[i]);
    free(d);
}

static inline size_t dict_hash_key(str key) {
    size_t hash = 5381;
    for (size_t i = 0; i < key->len; i++) {
        hash = ((hash << 5) + hash) + key->data[i];
    }
    return hash;
}

static inline void dict_grow_rehash(dict* d, size_t new_size) {
    dict new = dict_init(new_size);
    for (size_t i = 0; i < (*d)->size; i++) {
        dict_node n = (*d)->buckets[i];
        while (n != NULL) {
            dict_insert(&new, n->key, n->value);
            n = n->next;
        }
    }
    dict_free(*d);
    *d = new;
}

void dict_concat(dict* x, dict y) {
    if ((*x)->used + y->used > (*x)->size) dict_grow_rehash(x, ((*x)->size + y->used) * 2);
    for (size_t i = 0; i < y->used; i++) {
        dict_node n = y->buckets[i];
        while (n != NULL) {
            dict_insert(x, n->key, n->value);
            n = n->next;
        }
    }
}

void dict_insert(dict* d, str key, var value) {
    if ((*d)->used == (*d)->size) dict_grow_rehash(d, (*d)->size * 2);
    size_t pos = dict_hash_key(key) % (*d)->size;
    if ((*d)->buckets[pos] == NULL) {
        (*d)->buckets[pos] = dict_node_init(key, value);
        (*d)->used++;
        return;
    }
    dict_node pnode = (*d)->buckets[pos];
    for (;;) {
        if (str_cmp(pnode->key, key)) {
            pnode->value = value;
            return;
        }
        if (pnode->next == NULL) {
            pnode->next = dict_node_init(key, value);
            (*d)->used++;
            return;
        }
        pnode = pnode->next;
    }
}

bool dict_remove(dict d, var* err, str key, var* value) {
    size_t pos = dict_hash_key(key) % d->size;
    dict_node pnode = d->buckets[pos];
    if (pnode == NULL) {
        *err = var_err_c("Key Not Found");
        return false;
    }
    if (str_cmp(pnode->key, key)) {
        *value = pnode->value;
        d->buckets[pos] = pnode->next;
        str_free(pnode->key);
        free(pnode);
        return true;
    }
    dict_node prev = pnode;
    dict_node next = pnode->next;
    while (next != NULL) {
        if (str_cmp(next->key, key)) {
            dict_node tmp = next;
            prev->next = next->next;
            *value = tmp->value;
            str_free(tmp->key);
            free(tmp);
            return true;
        }
        prev = next;
        next = next->next;
    }
    *err = var_err_c("Key Not Found");
    return false;
}

void dict_print(const dict d) {
    for (size_t i = 0; i < d->size; i++) {
        dict_node n = d->buckets[i];
        while (n != NULL) {
            str_print(n->key);
            printf(" | ");
            var_print(n->value);
            putchar('\n');
            n = n->next;
        }
    }
}
