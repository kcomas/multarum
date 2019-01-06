
#include "str.h"
#include "test.h"

static void print_name(const char* name, str s) {
    printf("%s: ", name);
    str_print(s);
    printf("\n");
}

int main(void) {
    str cs = str_from_c("test");
    test("CS length is 4", cs->len == 4);
    print_name("CS", cs);
    str_push(&cs, utf8_from_c('a'));
    str_push(&cs, utf8_from_c('b'));
    str_push(&cs, utf8_from_c('c'));
    test("CS length is 5", cs->len == 7);
    print_name("CS", cs);
    str cs2 = str_from_c("test");
    str csb = str_from_c("abc");
    str_concat(&cs2, csb);
    str_free(csb);
    test("CS2 == CS", str_cmp(cs, cs2));
    print_name("CS2", cs2);
    str_free(cs2);
    str_free(cs);
    str i = str_from_c("abc");
    utf8 c;
    str_push(&i, utf8_init(0xce, 0xa3, 0, 0));
    str_iter si;
    str_iter_init(&si, i);
    test("First peek", str_iter_peek(&si, &c));
    test("First peek is a", utf8_cmp(c, utf8_from_c('a')));
    str_iter_next(&si, &c); // a
    str_iter_next(&si, &c); // b
    str_iter_next(&si, &c); // c
    test("Last peek", str_iter_peek(&si, &c));
    test("Last peek is sigma", utf8_cmp(c, utf8_init(0xce, 0xa3, 0, 0)));
    test("Last next", str_iter_next(&si, &c));
    test("Last next is sigma", utf8_cmp(c, utf8_init(0xce, 0xa3, 0, 0)));
    test("No more chars", str_iter_peek(&si, &c) == false);
    test("No more chars", str_iter_next(&si, &c) == false);
    str ic = str_copy(i);
    test("i and ic are the same string", str_cmp(i, ic));
    test("i and ic are different pointers", i != ic);
    str_free(ic);
    str_free(i);
}