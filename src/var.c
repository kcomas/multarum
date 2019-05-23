
#include "var.h"

void var_free(var v) {
    switch (v.type) {
        case VAR_STR:
            str_free(v.value.s);
            break;
        case VAR_HASH:
            hash_free(v.value.h, true);
            break;
        default:
            return;
    }
}

var var_copy(var v) {
    switch(v.type) {
        case VAR_STR:
            return var_str(str_copy(v.value.s, v.value.s->len - 1));
        case VAR_HASH:
            return var_hash(hash_copy(v.value.h, v.value.h->size, true));
        default:
            return v;
    }
}
