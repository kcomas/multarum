
#include "file.h"

bool file_init(str pathname, int flags, var* err, vfd* fd) {
    char* p = str_to_c(pathname);
    (*fd) = (vfd) malloc(sizeof(struct _vfd));
    (*fd)->ref_count = 1;
    (*fd)->fd  = open(p, flags);
    free(p);
    if ((*fd)->fd == -1) {
        free(*fd);
        *err = var_err_c("Failed To Open File");
        return false;
    }
    return true;
}

void file_free(vfd fd) {
    if (--fd->ref_count == 0) {
        close(fd->fd);
        free(fd);
    }
}

bool file_to_str(str pathname, var* err, str* s) {
    struct stat st;
    char* p = str_to_c(pathname);
    if ((stat(p, &st)) == -1) {
        *err = var_err_c("Failed To Lookup File");
        return false;
    }
    int fd;
    if  ((fd = open(p, O_RDWR | O_CREAT)) == -1) {
        *err = var_err_c("Failed To Open File");
        return false;
    }
    free(p);
    (*s) = str_init(st.st_size);
    if (read(fd, (*s)->data, st.st_size) < 0) {
        *err = var_err_c("Failed To Read File");
        return false;
    }
    if (close(fd) == -1) {
        *err = var_err_c("Failed To Close File");
        return false;
    }
    return true;
}
