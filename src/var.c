
#include "var.h"

void var_free(var v) {
    switch (v.type) {
        case VEC:
            vec_free(v.value.v);
            break;
        case STR:
            str_free(v.value.s);
            break;
        case FD:
            file_free(v.value.fd);
            break;
        case DICT:
            dict_free(v.value.d);
        default:
            break;
    }
}

void var_inc_ref(var v) {
    switch (v.type) {
        case VEC:
            v.value.v->ref_count++;
            break;
        case STR:
            v.value.s->ref_count++;
            break;
        case FD:
            v.value.fd->ref_count++;
            break;
        case DICT:
            v.value.d->ref_count++;
            break;
        default:
            break;
    }
}

bool var_cmp(var x, var y) {
    if (x.type != y.type) return false;
    switch (x.type) {
        case VEC:
            return vec_cmp(x.value.v, y.value.v);
        case BOOL:
            return x.value.b == y.value.b;
        case INT:
            return x.value.i == y.value.i;
        case FLOAT:
            return x.value.f == y.value.f;
        case UTF8:
            return utf8_cmp(x.value.c, y.value.c);
        case STR:
            return str_cmp(x.value.s, y.value.s);
        case FD:
            return x.value.fd->fd == y.value.fd->fd;
        default:
            return false;
    }
}

void var_print(const var v) {
    switch (v.type) {
        case VEC:
            vec_print(v.value.v);
            break;
        case BOOL:
            printf(v.value.b == true ? "1b" : "0b");
            break;
        case INT:
            printf("%li", v.value.i);
            break;
        case FLOAT:
            printf("%lf", v.value.f);
            break;
        case UTF8:
            utf8_print(v.value.c);
            break;
        case STR:
            str_print(v.value.s);
            break;
        case FD:
            printf("File %d", v.value.fd->fd);
            break;
        default:
            break;
    }
}
