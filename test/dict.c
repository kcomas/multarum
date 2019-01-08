
#include "dict.h"
#include "test.h"

int main(void) {
    dict d = dict_init(1);
    str keya = str_from_c("test");
    vec v = vec_init(2);
    vec_push(&v, var_int(1));
    vec_push(&v, var_int(2));
    dict_insert(&d, keya, var_vec(v));
    str keyb = str_from_c("test2");
    str vb = str_from_c("Some Value");
    dict_insert(&d, keyb, var_str(vb));
    dict_print(d);
    test("D has 2 items", d->used == 2);
    str_free(keya);
    str_free(keyb);
    str_free(vb);
    vec_free(v);
    dict_free(d);
    return 0;
}
