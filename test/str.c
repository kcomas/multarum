
#include "str.h"
#include "test.h"

int main(void) {
    str cs = str_from_c("test");
    test("CS length is 4", cs->len, ==, 4);
    str_free(cs);
}
