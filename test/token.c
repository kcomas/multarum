
#include "token.h"

int main(void) {
    char* err = NULL;
    token_state *ts = tokenize_file("./examples/math.mul", &err);
    if (ts == NULL) {
        printf("%s\n", err);
        return 1;
    }
    printf("%s\n", ts->str);
    token t;
    while (next_token(ts, &t, &err) && err == NULL) print_token(&t);
    if (err) {
        printf("%s\n", err);
        return 1;
    }
    free(ts);
    return 0;
}
