
#include "file.h"
#include "test.h"

int main(void) {
    str s = str_from_c("This is a test\n");
    str_push(&s, utf8_init(0xce, 0xa3, 0, 0));
    str n = str_from_c("test_file");
    var err;
    vfd f;
    file_init(n, O_RDWR | O_CREAT, &err, &f);
    str_free(n);
    str_free(s);
    return 0;
}
