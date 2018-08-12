
#include "file.h"

mt_var mt_open_file(mt_var* const path) {
    if (path->type != mt_pfx(BUFFER)) {
        return mt_var_err(mt_err_type());
    }
    int32_t fd = open((char*) path->data.mt_buf->data, O_RDONLY);
    if (fd == -1) {
        return mt_var_err(mt_err_init(mt_pfx(ERR_FILE_OPEN), errno, 0, NULL, mt_buf_from_c_str("Failed To Open File")));
    }
    return mt_var_file(fd);
}
