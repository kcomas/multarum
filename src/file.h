
#ifndef MT_FILE
#define MT_FILE

#include <fcntl.h>
#include <unistd.h>
#include "var.h"

mt_var mt_open_file(mt_buf* const path);

mt_var mt_read_file_chunk(int32_t file, mt_buf* const buf);

#endif
