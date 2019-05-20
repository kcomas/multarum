

#include "file.h"

int main(void) {
    err e;
    err_init(e);
    str *s = file_to_str("examples/fib.mul", &e);
    printf("%s\n", s->data);
    str_free(s);
    return 0;
}
