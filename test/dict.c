
#include "dict.h"

int main(void) {
    dict d = dict_init(1);
    dict_free(d);
    return 0;
}
