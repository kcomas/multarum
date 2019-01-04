
#include "var.h"

void var_free(var* v) {
    if (v->ref_count < 0 || --v->ref_count > 0) return;
    switch (v->type) {
        case 0:
            vec_free(v->value.v);
            break;
        case 4:
            str_free(v->value.s);
            break;
        default:
            break;
    }
}
