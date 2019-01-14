
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
    var value;
    var err;
    dict_get(d, &err, keyb, &value);
    test("D of test2 is a string", var_cmp(value, var_str(vb)));
    var_free(value);
    dict d2 = dict_init(1);
    str key3 = str_from_c("3");
    dict_insert(&d, key3, var_utf8(utf8_from_c('z')));
    dict_concat(&d, d2);
    dict_insert_c(&d, "woot", var_int(4));
    test("D has 3 items", d->used == 4);
    var get;
    dict_get_c(d, &err, "woot", &get);
    test("D of woot is 4", var_cmp(get, var_int(4)));
    dict_print(d);
    var removed;
    dict_remove(d, &err, keya, &removed);
    test("D has 2 itmes", d->used == 3);
    test("Removed item was v", var_cmp(removed, var_vec(v)));
    dict_print(d);
    var_free(removed);
    dict_free(d2);
    str_free(key3);
    str_free(keya);
    str_free(keyb);
    str_free(vb);
    vec_free(v);
    dict_free(d);
    return 0;
}
