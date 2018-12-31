
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

}

str str_from_c(char* c_str) {
    size_t size = strlen(c_str);
    str s = str_init(size);
    s->len = size;
    memcpy(s->data, c_str, size);
    return s;
}
