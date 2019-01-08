
#ifndef FILE_H
#define FILE_H

#include <sys/stat.h>
#include <fcntl.h>
#include "var.h"

int open_file(str pathname);

bool file_to_str(int fd, var* err, str* s);

#endif
