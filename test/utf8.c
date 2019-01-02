
#include "utf8.h"
#include "test.h"

int main(void) {
    test("Int value of a is 97", utf8_value(utf8_from_c('a')) == 97);
    utf8 sigma = utf8_init(0xce, 0xa3, 0, 0);
    test("Sigma length is 2", utf8_len(sigma) == 2);
    printf("Sigma: ");
    utf8_print(sigma);
    printf("\n");
    test("Sigma value is 931", utf8_value(sigma) == 931);
    utf8 a = utf8_from_c('a');
    utf8 b = utf8_from_c('b');
    test("a != b", utf8_cmp(a, b) == false);
    return 0;
}
