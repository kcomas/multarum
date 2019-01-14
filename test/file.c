
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
    test("File Seek", file_seek(f, &err, 0, SEEK_SET));
    dict d;
    test("File Stat", file_stat(f, &err, &d));
    dict_print(d);
    var size;
    dict_get_c(d, &err, "size", &size);
    test("File is 18 bytes", var_cmp(size, var_int(18)));
    str r;
    test("File Read", file_read(f, &err, &r));
    test("File read back in is same", str_cmp(s, r));
    test("File Delete", file_delete(f->pathname, &err));
    dict_free(d);
    file_free(f);
    str_free(n);
    str_free(s);
    str_free(r);
    str p = str_from_c("./examples");
    vec v;
    test("List files in dir", file_dir_list(p, &err, &v));
    test("More then 0 files", v->len > 0);
    vec_print(v);
    str_free(p);
    vec_free(v);
    printf("\n");
    return 0;
}
