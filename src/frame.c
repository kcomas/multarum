
#include "frame.h"

mt_frame* mt_frame_top_copy(const mt_frame* const frames, size_t f_len, size_t count) {
    if (f_len < 1) {
        return NULL;
    }
    mt_frame* new_frames = (mt_frame*) malloc(sizeof(mt_frame) * count);
    for (; count > 0; count--) {
        new_frames[count - 1] = frames[--f_len];
        new_frames[count - 1].mod->ref_count++;
    }
    return new_frames;
}
