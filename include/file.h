
#ifndef FILE_H
#define FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "var.h"

typedef struct _vfd {
    int fd;
    struct stat st;
    uint16_t ref_count;
} *vfd;

bool file_init(str pathname, int flags, var* err, vfd* fd);

void file_free(vfd fd);

bool file_to_str(str pathname, var* err, str* s);

#endif
