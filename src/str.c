
#include "str.h"

str str_init(size_t size) {
    str s = (str) malloc(sizeof(struct _str) + size * sizeof(uint8_t)); // redundant but shows size
    s->len = 0;
    s->size = size;
    return s;
}

void str_free(str s) {
    free(s);
}

void str_push(str* s, utf8 c) {
    if ((*s)->size - (*s)->len < utf8_len(c)) {
        str new = str_init((*s)->size * 2);
        new->len = (*s)->len;
        memcpy(new->data, (*s)->data, new->len);
        free(*s);
        *s = new;
    }
    size_t utf8len = utf8_len(c);
    memcpy((*s)->data + (*s)->len, c.b, utf8len);
    (*s)->len += utf8len;
}

str str_concat(str x, str y) {
    size_t len = x->len + y->len;
    str s = str_init(len * 2);
    s->len = len;
    memcpy(s->data, x->data, x->len);
    memcpy(s->data + x->len, y->data, y->len);
    return s;
}

str str_from_c(const char* const c_str) {
    size_t size = strlen(c_str);
    str s = str_init(size);
    s->len = size;
    memcpy(s->data, c_str, size);
    return s;
}

bool str_cmp(const str x, const str y) {
    if (x->len != y->len) return false;
    for (size_t i = 0; i < x->len; i++) if (x->data[i] != y->data[i]) return false;
    return true;
}

void str_print(const str s) {
    for (size_t i = 0; i < s->len; i++) putchar(s->data[i]);
}

static size_t str_next_c(str_iter* const si, utf8* c) {
    if (si->idx == si->s->len) return 0;
    size_t conts = utf8_cont(si->s->data[si->idx]);
    if (conts == 0) return 0;
    memset(c->b, 0, 4);
    memcpy(c->b, si->s->data + si->idx, conts);
    if (utf8_value(*c) == -1) {
        memset(c->b, 0, 4);
        return 0;
    }
    return conts;
}

void str_iter_init(str_iter* const si, str s) {
    si->idx = 0;
    si->s = s;
}

bool str_iter_peek(str_iter* const si, utf8* c) {
    if (str_next_c(si, c) == 0) return false;
    return true;
}

bool str_iter_next(str_iter* const si, utf8* c) {
    size_t conts = str_next_c(si, c);
    if (conts == 0) return false;
    si->idx += conts;
    return true;
}
