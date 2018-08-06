
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
