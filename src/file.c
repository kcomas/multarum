
#include "file.h"

mt_var mt_file_open(mt_buf* const path) {
    int fd = open((char*) path->data, O_RDONLY);
    if (fd == -1) {
        return mt_var_err(mt_err_file_open());
    }
    return mt_var_file(fd);
}

mt_var mt_read_file_chunk(int file, mt_buf* const buf) {
    ssize_t total = read(file, buf->data, buf->_size);
    if (total == -1) {
        return mt_var_err(mt_err_file_read());
    }
    buf->len = (size_t) total;
    return mt_var_int((int64_t) total);
}

mt_var mt_file_close(int file) {
    int rst = close(file);
    if (rst == -1) {
        return mt_var_err(mt_err_file_close());
    }
    return mt_var_bool(true);
}
