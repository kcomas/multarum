
#include "file.h"

mt_var mt_file_open(mt_buf* const path) {
    int fd = open((char*) path->data, O_RDONLY);
    if (fd == -1) {
        return mt_var_err(mt_err_init(mt_pfx(ERR_FILE_OPEN), errno, 0, NULL, mt_buf_from_c_str("Failed To Open File")));
    }
    return mt_var_file(fd);
}

mt_var mt_read_file_chunk(int file, mt_buf* const buf) {
    ssize_t total = read(file, buf->data, buf->_size);
    if (total == -1) {
        return mt_var_err(mt_err_init(mt_pfx(ERR_FILE_READ), errno, 0, NULL, mt_buf_from_c_str("File Read Fail")));
    }
    buf->len = (size_t) total;
    return mt_var_int((int64_t) total);
}

mt_var mt_file_close(int file) {
    int rst = close(file);
    if (rst == -1) {
        return mt_var_err(mt_err_init(mt_pfx(ERR_FILE_READ), errno, 0, NULL, mt_buf_from_c_str("Failed To Close File")));
    }
    return mt_var_bool(true);
}
