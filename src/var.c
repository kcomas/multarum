
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

void var_print(const var v) {
    switch (v.type) {
        case 0:
            vec_print(v.value.v);
            break;
        case -1:
            printf(v.value.b == true ? "1b" : "0b");
            break;
        case -2:
            printf("%li", v.value.i);
            break;
        case -3:
            printf("%lf", v.value.f);
            break;
        case -4:
            utf8_print(v.value.c);
            break;
        case 4:
            str_print(v.value.s);
            break;
        default:
            break;
    }
}
