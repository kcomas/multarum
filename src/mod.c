
#include "mod.h"

mt_mod* mt_mod_init(size_t _size) {
    mt_mod* mod = (mt_mod*) malloc(sizeof(mt_mod));
    mod->_size = _size;
    mod->len = 0;
    mod->bytes = (uint8_t*) malloc(_size);
    return mod;
}

void mt_mod_free(mt_mod* const mod) {
    free(mod->bytes);
    free(mod);
}

void mt_mod_dis(const mt_mod* const mod) {
    mt_op_str_init();
    int32_t mt_jmp;
    for (size_t i = 0; i < mod->len; i++) {
        printf("<%p> %lu: ", &mod->bytes[i], i);
        switch (mod->bytes[i]) {
            case MT_NOP:
            case MT_ADD:
            case MT_SUB:
            case MT_RET:
            case MT_HALT:
                mt_mod_dist_op(mod, i);
                break;
            case MT_JMP:
                for (size_t x = i; x < i + 5; x++) {
                    printf("0x%x ", mod->bytes[x]);
                }
                memcpy(&mt_jmp, mod->bytes + i + 1, sizeof(int32_t));
                printf("%s %d\n", mt_op_str(MT_JMP), mt_jmp);
                i += 5;
                break;
            default:
                printf("\n");
        }
    }
}
