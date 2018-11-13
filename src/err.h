
#ifndef MT_ERR_H
#define MT_ERR_H

#include <stdint.h>
#include "common.h"
#include <errno.h>
#include "mod.h"
#include "buf.h"

typedef struct _mt_frame mt_frame;

#define mt_pfx_err(NAME) mt_pfx(_ERR_##NAME)

typedef enum {
    mt_pfx_err(OK),
    mt_pfx_err(TYPE),
    mt_pfx_err(FILE_OPEN),
    mt_pfx_err(FILE_READ),
    mt_pfx_err(BUF_FULL),
    mt_pfx_err(HASH_KEY_LEN),
    mt_pfx_err(AST_BUILD),
    mt_pfx_err(DUP_ARG),
    mt_pfx_err(TOKEN_END),
    mt_pfx_err(IF_TOKEN_END),
    mt_pfx_err(CALL_TOKEN_END)
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

#define mt_err_type_err() mt_err_init(mt_pfx_err(ERR_TYPE), 0, 0, NULL, mt_buf_from_c_str("Invalid Type"))

#define mt_err_file_open() \
    mt_err_init(mt_pfx_err(FILE_OPEN), errno, 0, NULL, mt_buf_from_c_str("Failed To Open File"))

#define mt_err_file_read() \
    mt_err_init(mt_pfx_err(FILE_READ), errno, 0, NULL, mt_buf_from_c_str("File Read Fail"))

#define mt_err_file_close() \
    mt_err_init(mt_pfx_err(FILE_READ), errno, 0, NULL, mt_buf_from_c_str("Failed To Close File"))

#define mt_err_token_buf_full() \
    mt_err_init(mt_pfx_err(BUF_FULL), 0, 0, NULL, mt_buf_from_c_str("Token Data Buf Full"))

#define mt_err_hash_key_len() \
    mt_err_init(mt_pfx_err(HASH_KEY_LEN), 0, 0, NULL, mt_buf_from_c_str("Hash Key Len Exceed"))

#define mt_err_ast_build_fail(err) \
    mt_err_init(mt_pfx_err(AST_BUILD), 0, 0, NULL, err)

#define mt_err_ast_dup_arg() \
    mt_err_init(mt_pfx_err(DUP_ARG), 0, 0, NULL, mt_buf_from_c_str("Duplicate Fn Arg Found"))

#define mt_err_ast_token_end() \
    mt_err_init(mt_pfx_err(TOKEN_END), 0, 0, NULL, mt_buf_from_c_str("No More Tokens"))

#define mt_err_ast_invalid_if_state() \
    mt_err_init(mt_pfx_err(IF_TOKEN_END), 0, 0, NULL, mt_buf_from_c_str("Invalid If, No More Tokens"))

#define mt_err_ast_invalid_call_state() \
    mt_err_init(mt_pfx_err(CALL_TOKEN_END), 0, 0, NULL, mt_buf_from_c_str("Invalid Call, No More Tokens"))

void mt_err_free(mt_err* const err);

#endif
