
#ifndef FILE_H
#define FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "var.h"

typedef struct _rfd {
    uint16_t ref_count;
    int fd;
    str pathname;
} *rfd;

bool file_init(str pathname, int flags, mode_t mode, var* err, rfd* fd);

void file_free(rfd fd);

bool file_read(rfd fd, var* err, str* s);

bool file_seek(rfd fd, var* err, off_t offset, int whence);

bool file_write(rfd fd, var* err, str s);

bool file_delete(str pathname, var* err);

bool file_to_str(str pathname, var* err, str* s);

bool file_stat(rfd fd, var* err, dict* d);

bool file_dir_list(str pathname, var* err, vec* v);

#endif
