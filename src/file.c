
#include "file.h"

#define file_to_c_sub(s) \
    char p[256]; \
    str_to_c(s, p, s->len + 1)

bool file_init(str pathname, int flags, mode_t mode, var* err, rfd* fd) {
    file_to_c_sub(pathname);
    (*fd) = (rfd) malloc(sizeof(struct _rfd));
    (*fd)->ref_count = 1;
    (*fd)->fd  = open(p, flags, mode);
    (*fd)->pathname = pathname;
    pathname->ref_count++;
    if ((*fd)->fd == -1) {
        free(*fd);
        *err = var_err_c("Failed To Open File");
        return false;
    }
    return true;
}

void file_free(rfd fd) {
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
        if (read(fd, (*s)->data, st.st_size) == -1) { \
            *err = var_err_c("Failed To Read File"); \
            return false; \
        } \
        (*s)->len = st.st_size; \
    } while (0)

bool file_read(rfd fd, var* err, str* s) {
    struct stat st;
    file_to_c_sub(fd->pathname);
    file_stat_sub(p, &st);
    file_read_sub(fd->fd, s, st);
    return true;
}

bool file_seek(rfd fd, var* err, off_t offset, int whence) {
    if (lseek(fd->fd, offset, whence) == -1) {
        *err = var_err_c("Unable To Seek File");
        return false;
    }
    return true;
}

bool file_write(rfd fd, var* err, str s) {
    if (write(fd->fd, s->data, s->len) == -1) {
        *err = var_err_c("Failed To Write File");
        return false;
    }
    return true;
}

bool file_delete(str pathname, var* err) {
    file_to_c_sub(pathname);
    if (remove(p) == -1) {
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

bool file_stat(rfd fd, var* err, dict* d) {
    struct stat st;
    file_to_c_sub(fd->pathname);
    file_stat_sub(p, &st);
    (*d) = dict_init(15);
    dict_insert_c(d, "pathname", var_str(fd->pathname));
    dict_insert_c(d, "dev", var_int(st.st_dev));
    dict_insert_c(d, "ino", var_int(st.st_ino));
    dict_insert_c(d, "mode", var_int(st.st_mode));
    dict_insert_c(d, "nlink", var_int(st.st_nlink));
    dict_insert_c(d, "uid", var_int(st.st_uid));
    dict_insert_c(d, "gid", var_int(st.st_gid));
    dict_insert_c(d, "size", var_int(st.st_size));
    // @TODO add time fields
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
    while ((dp = readdir(dirp)) != NULL)
        if (strcmp(".", dp->d_name) && strcmp("..", dp->d_name)) {
            var s = var_str(str_from_c(dp->d_name));
            vec_push(v, s);
            var_free(s);
        }
    if (closedir(dirp) == -1) {
        *err = var_err_c("Failed To Close Directory");
        return false;
    }
    return true;
}
