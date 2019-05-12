
#include "file.h"

bool file_close(int fd, err *e) {
   if (close(fd) == -1) {
        err_system(e);
        return false;
   }
   return true;
}

str file_to_str(char *filename, err *e) {
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
        err_system(e);
        return NULL;
    }
    file_close(fd, e);
    return s;
}
