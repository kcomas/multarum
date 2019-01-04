
#include "vec.h"

vec vec_init(size_t size) {
    vec v = (vec) malloc(sizeof(struct _vec) + size * sizeof(var));
    v->size = size;
    v->len = 0;
    return v;
}

void vec_free(vec v) {
    for (size_t i = 0; i < v->len; i++) var_free(&v->data[i]);
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

void vec_concat(vec* x, vec y) {
    if ((*x)->size - (*x)->len < y->len) {
        vec new = vec_init(((*x)->len + y->len) * 2);
        new->len = (*x)->len;
        memcpy(new->data, (*x)->data, new->len);
        free(*x);
        *x = new;
    }
    memcpy((*x)->data + (*x)->len, y->data, y->len);
    (*x)->len += y->len;
}

vec vec_copy(vec v) {
    vec new = vec_init(v->len * 2);
    memcpy(new, v, sizeof(struct _vec) + v->len * sizeof(var));
    return new;
}

void vec_reverse(vec v) {
    for (size_t i = 0; i < v->len / 2; i++) {
        var tmp = v->data[i];
        v->data[i] = v->data[v->len - i];
        v->data[v->len - i] = tmp;
    }
}

bool vec_pop(vec v, var* err, var* value) {
    if (v->len == 0) {
        *err = var_err_c("Cannot Pop From Empty Vec");
        return false;
    }
    *value = v->data[--v->len];
    return true;
}

bool vec_insert(vec* v, var* err, size_t idx, var value) {
    if (idx > (*v)->len) {
        *err = var_err_c("Cannot Insert Into Index Higher Then Len");
        return false;
    }
    if (idx == (*v)->len && (*v)->len == (*v)->size) {
        vec new = vec_copy(*v);
        free(*v);
        *v = new;
    }
    (*v)->data[idx] = value;
    return true;
}

bool vec_remove(vec v, var* err, size_t idx, var* value) {
    if (idx >= v->len) {
        *err = var_err_c("Cannot Remove Index Higher Then Len - 1");
        return false;
    }
    *value = v->data[idx];
    for (size_t i = idx + 1; i < v->len; i++) {
        v->data[i - 1] = v->data[i];
    }
    v->len--;
    return true;
}

void vec_print(const vec v) {
    for (size_t i = 0; i < v->len; i++) var_print(v->data[i]);
}
