
#include "dict.h"

int main(void) {
    dict d = dict_init(1);
    str key = str_from_c("test");
    vec v = vec_init(2);
    vec_push(&v, var_int(1));
    vec_push(&v, var_int(2));
    dict_insert(&d, key, var_vec(v));
    dict_print(d);
    str_free(key);
    vec_free(v);
    dict_free(d);
    return 0;
}
