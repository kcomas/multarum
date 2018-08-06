
#include "err.h"

mt_err* mt_err_new(mt_err_type type, int32_t no, size_t f_len, mt_frame* const rsp, mt_buf* const msg) {
    mt_err* err = (mt_err*) malloc(sizeof(mt_err));
    err->type = type;
    err->ref_count = 1;
    err->no = no;
    err->f_len = f_len;
    err->rsp = rsp;
    err->msg = msg;
    return err;
}
