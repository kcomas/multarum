
#ifndef FILE_H
#define FILE_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "var.h"

bool file_to_str(str pathname, var* err, str* f);

#endif
