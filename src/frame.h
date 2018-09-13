
#ifndef MT_FRAME_H
#define MT_FRAME_H

#include <stdint.h>
#include "mod.h"

typedef struct _mt_frame {
    bool safe;
    uint8_t fn;
    uint32_t rbp;
    uint8_t* rip;
    mt_mod* mod;
} mt_frame;

mt_frame* mt_frame_top_copy(const mt_frame* const frames, size_t f_len, size_t count);

#endif
