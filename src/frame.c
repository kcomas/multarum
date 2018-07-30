
#include "frame.h"

mt_frame* mt_frame_top_copy(const mt_frame* const frames, size_t f_len, size_t count) {
    mt_frame* new_frames = (mt_frame*) malloc(sizeof(mt_frame) * count);
    while (count > 0) {
        new_frames[count - 1] = frames[f_len - 1];
        new_frames[count - 1].mod->ref_count++;
        f_len--;
        count--;
    }
    return new_frames;
}
