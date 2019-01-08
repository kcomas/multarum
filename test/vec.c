
#include "vec.h"
#include "test.h"

int main(void) {
    vec v = vec_init(1);
    vec_push(&v, var_int(1));
    vec_push(&v, var_int(2));
    test("Vec is length 2", v->len == 2);
    vec_push(&v, var_int(3));
    vec v2 = vec_init(1);
    vec_push(&v2, var_int(4));
    vec_concat(&v, v2);
    vec_free(v2);
    test("Vec is length 4", v->len == 4);
    var err;
    var value;
    vec_get(v, &err, 2, &value);
    test("V[2] is 3", var_cmp(value, var_int(3)));
    vec v3 = vec_copy(v);
    test("V3 is the same as V", vec_cmp(v, v3));
    vec_print(v3);
    printf("\n");
    free(v3);
    vec_reverse(v);
    vec_print(v);
    printf("\n");
    vec_pop(v, &err, &value);
    test("Vec is now length 3", v->len == 3);
    test("Removed value is 1", var_cmp(value, var_int(1)));
    vec_insert(&v, &err, 1, var_bool(true));
    test("V at 1 is a bool", var_cmp(v->data[1], var_bool(true)));
    vec_remove(v, &err, 0, &value);
    test("Vec is now length 2", v->len == 2);
    test("Removed value is 4", var_cmp(value, var_int(4)));
    vec_print(v);
    printf("\n");
    vec_free(v);
    return 0;
}
