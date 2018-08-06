
#ifndef MT_CTX
#define MT_CTX

#include "buf.h"

#define MT_DEFAULT_BUF_SIZE 100

typedef struct {
   mt_buf* read_buf;
   mt_buf* write_buf;
} mt_ctx;

#endif
