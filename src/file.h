
#ifndef MT_FILE_H
#define MT_FILE_H

#include <fcntl.h>
#include <unistd.h>
#include "var.h"

mt_var mt_file_open(mt_buf* const path);

mt_var mt_read_file_chunk(int file, mt_buf* const buf);

mt_var mt_file_close(int file);

#endif
