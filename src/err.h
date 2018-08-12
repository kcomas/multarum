
#ifndef MT_ERR
#define MT_ERR

#include <stdint.h>
#include "common.h"
#include <errno.h>
#include "mod.h"
#include "buf.h"

typedef struct _mt_frame mt_frame;

typedef enum {
    mt_pfx(ERR_OK),
    mt_pfx(ERR_TYPE),
    mt_pfx(ERR_FILE_OPEN)
} mt_err_type;

#ifndef MT_ERR_STACK_COPY
#   define MT_ERR_STACK_COPY 10
#endif

typedef struct _mt_err {
    mt_err_type type;
    uint32_t ref_count;
    int32_t no; // errno
    size_t f_len;
    mt_frame* rsp;
    mt_buf* msg;
} mt_err;

mt_err* mt_err_init(mt_err_type type, int32_t no, size_t f_len, mt_frame* const rsp, mt_buf* const msg);

#define mt_err_type() mt_err_init(mt_pfx(ERR_TYPE), 0, 0, NULL, mt_buf_from_c_str("Invalid Type"))

void mt_err_free(mt_err* const err);

#endif
