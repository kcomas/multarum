
#ifndef MT_CTX_H
#define MT_CTX_H

#include "token.h"

#ifndef MT_DEFAULT_CTX_BUF_SIZE
#   define MT_DEFAULT_CTX_BUF_SIZE 1000
#endif

typedef struct {
   mt_buf* read_buf;
   mt_buf* write_buf;
} mt_ctx;

void mt_ctx_init(mt_ctx* const ctx);

void mt_ctx_free(mt_ctx* const ctx);

#endif
