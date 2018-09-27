
#ifndef MT_ERR_H
#define MT_ERR_H

#include <stdint.h>
#include "common.h"
#include <errno.h>
#include "mod.h"
#include "buf.h"

typedef struct _mt_frame mt_frame;

typedef enum {
    mt_pfx(ERR_OK),
    mt_pfx(ERR_TYPE),
    mt_pfx(ERR_FILE_OPEN),
    mt_pfx(ERR_FILE_READ),
    mt_pfx(ERR_BUF_FULL),
    mt_pfx(ERR_HASH_KEY_LEN)
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

#define mt_err_type_err() mt_err_init(mt_pfx(ERR_TYPE), 0, 0, NULL, mt_buf_from_c_str("Invalid Type"))

#define mt_err_token_buf_full() \
    mt_err_init(mt_pfx(ERR_BUF_FULL), 0, 0, NULL, mt_buf_from_c_str("Token Data Buf Full"))

#define mt_err_hash_key_len() \
    mt_err_init(mt_pfx(ERR_HASH_KEY_LEN), 0, 0, NULL, mt_buf_from_c_str("Hash Key Len Exceed"))

void mt_err_free(mt_err* const err);

#endif
