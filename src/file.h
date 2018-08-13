
#ifndef MT_FILE
#define MT_FILE

#include <fcntl.h>
#include <unistd.h>
#include "var.h"

mt_var mt_file_open(mt_buf* const path);

mt_var mt_read_file_chunk(int32_t file, mt_buf* const buf);

mt_var mt_file_close(int32_t file);

#endif
