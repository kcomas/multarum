
#include "str.h"
#include "test.h"

int main(void) {
    str cs = str_from_c("test");
    test("CS length is 4", cs->len, ==, 4);
    printf("CS: ");
    str_print(cs);
    printf("\n");
    str_push(&cs, utf8_from_c('a'));
    str_push(&cs, utf8_from_c('b'));
    str_push(&cs, utf8_from_c('c'));
    test("CS length is 5", cs->len, ==, 7);
    printf("CS: ");
    str_print(cs);
    printf("\n");
    str_free(cs);
}
