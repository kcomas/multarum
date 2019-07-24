
#include "token.h"

int main(void) {
    char* err;
    token_state *ts = tokenize_file("./examples/fib.mul", &err);
    if (ts == NULL) {
        printf("%s\n", err);
        return 1;
    }
    return 0;
}
