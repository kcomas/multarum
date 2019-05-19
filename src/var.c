
#include "var.h"

void var_free(var v) {
    switch (v.type) {
        case VAR_HASH:
            hash_free(v.value.h);
            break;
        default:
            return;
    }
}
