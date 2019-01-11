
#ifndef FILE_H
#define FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include "var.h"

typedef struct _vfd {
    uint16_t ref_count;
    int fd;
    str pathname;
} *vfd;

bool file_init(str pathname, int flags, var* err, vfd* fd);

void file_free(vfd fd);

bool file_to_str(str pathname, var* err, str* s);

bool file_stat(vfd file, var* err, dict* d);

bool file_dir_list(str pathname, var* err, vec* v);

#endif
