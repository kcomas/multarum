
#include "utf8.h"
#include "test.h"

int main(void) {
    test("Int value of a is 97", utf8_value(utf8_from_c('a')), ==, 97);
    return 0;
}
