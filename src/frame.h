
#ifndef MT_FRAME
#define MT_FRAME

#include <stdint.h>
#include "mod.h"

typedef struct _mt_frame {
    bool safe;
    mt_fn_place fn;
    uint32_t rbp;
    uint8_t* rip;
    mt_mod* mod;
} mt_frame;

mt_frame* mt_frame_top_copy(const mt_frame* const frames, size_t f_len, size_t count);

#endif
