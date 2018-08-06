
#include "file.h"

mt_var mt_open_file(char* const path) {
    int32_t fd = open(path, O_RDONLY);
    if (fd == -1) {
        return mt_var_err(mt_err_init(mt_pfx(ERR_FILE_OPEN), errno, 0, 0, mt_buf_from_c_str("Failed To Open File")));
    }
    return mt_var_file(fd);
}
