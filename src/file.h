
#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include "str.h"
#include "err.h"

bool file_close(int fd, err *const e);

str *file_to_str(char *const filename, err *const e);
