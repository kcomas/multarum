
#include "var.h"

void var_free(var v) {
    switch (v.type) {
        case 0:
            vec_free(v.value.v);
            break;
        case 4:
            str_free(v.value.s);
            break;
        default:
            break;
    }
}

bool var_cmp(var x, var y) {
    if (x.type != y.type) return false;
    switch (x.type) {
        case 0:
            return vec_cmp(x.value.v, y.value.v);
        case -1:
            return x.value.b == y.value.b;
        case -2:
            return x.value.i == y.value.i;
        case -3:
            return x.value.f == y.value.f;
        case -4:
            return utf8_cmp(x.value.c, y.value.c);
        case 4:
            return str_cmp(x.value.s, y.value.s);
        default:
            return false;
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
