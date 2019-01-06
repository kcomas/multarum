
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

static inline vec vec_resize_copy(vec v, size_t multipler) {
    vec new = vec_init(v->size * multipler);
    new->len = v->len;
    for (size_t i = 0; i < new->len; i++) {
        new->data[i] = v->data[i];
    }
    return new;
}

void vec_push(vec* v, var value) {
    if ((*v)->len == (*v)->size) {
        vec new = vec_resize_copy(*v, 2);
        free(*v);
        *v = new;
    }
    (*v)->data[(*v)->len++] = value;
}

void vec_concat(vec* x, vec y) {
    if ((*x)->size - (*x)->len < y->len) {
        vec new = vec_resize_copy(*x, 2);
        free(*x);
        *x = new;
    }
    for (size_t i = 0; i < y->len; i++) {
        (*x)->data[(*x)->len + i] = y->data[i];
    }
    (*x)->len += y->len;
}

vec vec_copy(vec v) {
    return vec_resize_copy(v, 1);
}

void vec_reverse(vec v) {
    size_t start = 0;
    size_t end = v->len - 1;
    while (start < end) {
        var tmp = v->data[start];
        v->data[start--] = v->data[end];
        v->data[end--] = tmp;
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

bool vec_cmp(vec x, vec y) {
    if (x->len != y->len) return false;
    for (size_t i = 0; i < x->len; i++) if (!var_cmp(x->data[i], y->data[i])) return false;
    return true;
}

void vec_print(const vec v) {
    for (size_t i = 0; i < v->len; i++) {
        var_print(v->data[i]);
        putchar(' ');
    }
}
