
#include "file.h"
#include "test.h"

int main(void) {
    str s = str_from_c("This is a test\n");
    str_push(&s, utf8_init(0xce, 0xa3, 0, 0));
    str_push(&s, utf8_from_c('\n'));
    str n = str_from_c("test_file");
    var err;
    rfd f;
    test("File Init", file_init(n, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &err, &f));
    test("F is valid file", f->fd != -1);
    test("File Write", file_write(f, &err, s));
    file_free(f);
    str r;
    rfd f2;
    test("File Init 2", file_init(n, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH, &err, &f2));
    test("File Read", file_read(f2, &err, &r));
    test("File read back in is same", str_cmp(s, r));
    test("File Delete", file_delete(f2->pathname, &err));
    file_free(f2);
    str_free(n);
    str_free(s);
    str_free(r);
    return 0;
}
