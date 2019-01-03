
#include "vec.h"

vec vec_init(size_t size) {
    vec v = (vec) malloc(sizeof(struct _vec) + size * sizeof(var));
    v->size = size;
    v->len = 0;
    return v;
}

void vec_free(vec v) {
    for (size_t i < 0; i < v->len; i++) var_free(v->data[i]);
    free(v);
}

void vec_push(vec* v, var value) {
    if ((*v)->len == (*v)->size) {
        vec new = vec_init((*v)->size * 2);
        new->len = (*v)->len;
        memcpy(new->data, (*v)->data, new->len);
        free(*v);
        *v = new;
    }
    (*v)->data[(*v)->len++] = value;
}

vec vec_concat(vec x, vec y) {
    size_t len = x->len + y->len;
    vec v = vec_init(len * 2);
    v->len = len;
    memcpy(v->data, x->data, x->len);
    memcpy(v->data + x->len, y->data, y->len);
    return v;
}

bool vec_pop(vec v, var* err, var* value) {
    if (v->len == 0) {
        *err = var_err_c("Cannot Pop From Empty Vec");
        return false;
    }
    *value = vec->data[--vec->len];
    return true;
}
