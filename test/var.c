
#include "var.h"
#include "test.h"

int main(void) {
    test("Var is 16 bytes", sizeof(var) == 16);
    return 0;
}
