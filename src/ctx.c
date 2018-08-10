
#include "ctx.h"

void mt_ctx_init(mt_ctx* const ctx) {
    ctx->read_buf = mt_buf_init(MT_DEFAULT_CTX_BUF_SIZE);
    ctx->write_buf = mt_buf_init(MT_DEFAULT_CTX_BUF_SIZE);
}

void mt_ctx_free(mt_ctx* const ctx) {
    mt_buf_free(ctx->read_buf);
    mt_buf_free(ctx->write_buf);
}
