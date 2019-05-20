
#include "str.h"

str *str_init(size_t size) {
    // null terminated strings with size
    str *s = calloc(1, sizeof(struct _str) + sizeof(char) * ++size);
    s->size = size;
    s->len = 0;
    return s;
}

void str_free(str *const s) {
    free(s);
}

str* str_clone(const str *const s, size_t size) {
    str *clone = str_init(size);
    clone->len = s->len;
    memcpy(clone->data, s->data, s->len);
    return clone;
}

str *str_from_cstring(char *cstring) {
    size_t len = strlen(cstring);
    str *s = str_init(len);
    s->len = len;
    memcpy(s->data, cstring, len);
    return s;
}
