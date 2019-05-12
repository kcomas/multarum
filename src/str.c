
#incude "str.h"

str str_init(size_t size) {
    str s = calloc(1, sizeof(struct _str) + sizeof(char) * size);
    s->size = size;
    s->len = 0;
    return s;
}

str str_from_cstring(char *cstring) {
    size_t len = strlen(cstring);
    str s = str_init(len);
    s->len = len;
    memcpy(s->data, cstring, len);
    return s;
}
