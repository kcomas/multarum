
#include "buf.h"

mt_buf* mt_buf_init(size_t _size) {
    mt_buf* buf = (mt_buf*) malloc(sizeof(mt_buf));
    buf->len = 0;
    buf->_size = _size;
    buf->data = (uint8_t*) malloc(buf->_size);
    return buf;
}

mt_buf* mt_buf_from_c_str(const char* const str) {
    size_t _size = strlen(str) + 1;
    mt_buf* buf = mt_buf_init(_size);
    memcpy(buf->data, str, _size);
    buf->len = _size;
    return buf;
}

void mt_buf_free(mt_buf* const buf) {
    free(buf->data);
    free(buf);
}

void mt_buf_zero(mt_buf* const buf) {
    memset(buf->data, 0, buf->len);
    buf->len = 0;
}

void mt_buf_iter_init(const mt_buf* const buf, mt_buf_iter* const iter) {
    iter->buf_pos = 0;
    iter->buf = buf;
}

static int8_t mt_buf_iter_n(mt_buf_iter* const iter, mt_char* const c) {
    if (iter->buf_pos >= iter->buf->len) {
        return -1;
    }
    int8_t conts = mt_char_cont(iter->buf->data[iter->buf_pos]);
    if (conts < 0) {
        return conts;
    }
    c->a = iter->buf->data[iter->buf_pos];
    if (conts >= 1) {
        c->b = iter->buf->data[iter->buf_pos + 1];
        if (conts >= 2) {
            c->c = iter->buf->data[iter->buf_pos + 2];
            if (conts == 3) {
                c->d = iter->buf->data[iter->buf_pos + 3];
            }
        }
    }
    return conts + 1;
}

bool mt_buf_iter_next(mt_buf_iter* const iter, mt_char* const c) {
    int8_t i = mt_buf_iter_n(iter, c);
    if (i < 0) {
        return false;
    }
    iter->buf_pos += i;
    return true;
}

bool mt_buf_iter_peek(mt_buf_iter* const iter, mt_char* const c) {
    int8_t i = mt_buf_iter_n(iter, c);
    return i > 0 ? true : false;
}

bool mt_buf_push_char(mt_buf* const buf, mt_char c) {
    int8_t conts = mt_char_cont(c.a);
    if (buf->len + conts + 1 > buf->_size) {
        return false;
    }
    buf->data[buf->len++] = c.a;
    if (conts >= 1) {
        buf->data[buf->len++] = c.b;
        if (conts >= 2) {
            buf->data[buf->len++] = c.c;
            if (conts == 3) {
                buf->data[buf->len++] = c.d;
            }
        }
    }
    return true;
}

void mt_buf_debug_print(const mt_buf* const buf) {
    for (size_t i = 0; i < buf->len; i++) {
        putchar(buf->data[i]);
    }
}

