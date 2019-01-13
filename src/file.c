
#include "file.h"

#define file_to_c_sub(s) \
    char p[256]; \
    str_to_c(s, p, s->len + 1)

bool file_init(str pathname, int flags, var* err, vfd* fd) {
    file_to_c_sub(pathname);
    (*fd) = (vfd) malloc(sizeof(struct _vfd));
    (*fd)->ref_count = 1;
    (*fd)->fd  = open(p, flags);
    (*fd)->pathname = pathname;
    pathname->ref_count++;
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
        str_free(fd->pathname);
        free(fd);
    }
}

#define file_stat_sub(p, st) \
    do { \
        if ((stat(p, st)) == -1) { \
            *err = var_err_c("Failed To Lookup File"); \
            return false; \
        } \
    } while (0)

#define file_read_sub(fd, s, st) \
    do { \
        (*s) = str_init(st.st_size); \
        if (read(fd, (*s)->data, st.st_size) < 0) { \
            *err = var_err_c("Failed To Read File"); \
            return false; \
        } \
    } while (0)

bool file_read(vfd fd, var* err, str* s) {
    struct stat st;
    file_to_c_sub(fd->pathname);
    file_stat_sub(p, &st);
    file_read_sub(fd->fd, s, st);
    return true;
}

bool file_write(vfd fd, var* err, str s) {
    if (!write(fd->fd, s->data, s->len)) {
        *err = var_err_c("Failed To Write File");
        return false;
    }
    return true;
}

bool file_delete(str pathname, var* err) {
    file_to_c_sub(pathname);
    if (!remove(p)) {
        *err = var_err_c("Failed To Remove File");
        return false;
    }
    return true;
}

bool file_to_str(str pathname, var* err, str* s) {
    struct stat st;
    file_to_c_sub(pathname);
    file_stat_sub(p, &st);
    int fd;
    if  ((fd = open(p, O_RDWR | O_CREAT)) == -1) {
        *err = var_err_c("Failed To Open File");
        return false;
    }
    file_read_sub(fd, s, st);
    if (close(fd) == -1) {
        *err = var_err_c("Failed To Close File");
        return false;
    }
    return true;
}

bool file_stat(vfd file, var* err, dict* d) {
    struct stat st;
    file_to_c_sub(file->pathname);
    file_stat_sub(p, &st);
    (*d) = dict_init(15);
    return true;
}

bool file_dir_list(str pathname, var* err, vec* v) {
    DIR *dirp;
    file_to_c_sub(pathname);
    if ((dirp = opendir(p)) == NULL) {
        *err = var_err_c("Failed To Open Directory");
        return false;
    }
    struct dirent *dp;
    (*v) = vec_init(5);
    while ((dp = readdir(dirp)) != NULL) {
        vec_push(v, var_str(str_from_c(dp->d_name)));
    }
    if (closedir(dirp) == -1) {
        *err = var_err_c("Failed To Close Directory");
        return false;
    }
    return true;
}
