
#include "file.h"

bool file_close(int fd, err *const e) {
   if (close(fd) == -1) {
        err_system(e);
        return false;
   }
   return true;
}

str file_to_str(char *const filename, err *const e) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        err_system(e);
        return NULL;
    }
    struct stat info;
    if (fstat(fd, &info) == -1) {
        err_system(e); // does not close to get err
        return NULL;
    }
    str s = str_init(info.st_size);
    if (read(fd, s->data, info.st_size) != (ssize_t) info.st_size) {
        str_free(s);
        err_system(e);
        return NULL;
    }
    s->len = info.st_size;
    file_close(fd, e);
    return s;
}
