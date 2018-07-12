
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

static void mt_print_byte_hex(const mt_mod* const mod, size_t i, size_t total) {
    for (size_t x = i; x < i + total; x++) {
        printf("0x%x ", mod->bytes[x]);
    }
}

void mt_mod_dis(const mt_mod* const mod) {
    mt_op_str_init();
    int32_t mt_jmp;
    int64_t mt_int;
    double mt_float;
    size_t i = 0;
    while (i < mod->len) {
        printf("<%p> %lu: ", &mod->bytes[i], i);
        switch (mod->bytes[i]) {
            case MT_NOP:
            case MT_ADD:
            case MT_SUB:
            case MT_RET:
            case MT_HALT:
                mt_mod_dist_op(mod, i);
                i++;
                break;
            case MT_PUSH:
                mt_print_byte_hex(mod, i, 1);
                switch (mod->bytes[++i]) {
                    case MT_NULL:
                        mt_print_byte_hex(mod, i, 2);
                        printf("PUSH NULL\n");
                        i += 2;
                        break;
                    case MT_BOOL:
                        mt_print_byte_hex(mod, i, 2);
                        printf("PUSH %s\n", mod->bytes[++i] == true ? "true" : "false");
                        i++;
                        break;
                    case MT_CHAR:
                        mt_print_byte_hex(mod, i, 2);
                        printf("PUSH %c\n", mod->bytes[++i]);
                        i++;
                        break;
                    case MT_INT:
                        mt_print_byte_hex(mod, i, 9);
                        mt_mod_get_bytes(mod, &mt_int, sizeof(int64_t), i);
                        printf("PUSH %li\n", mt_int);
                        break;
                    case MT_FLOAT:
                        mt_print_byte_hex(mod, i, 9);
                        mt_mod_get_bytes(mod, &mt_float, sizeof(double), i);
                        printf("PUSH %lf\n", mt_float);
                        break;
                }
                break;
            case MT_JMP:
                mt_print_byte_hex(mod, i, 5);
                i++;
                memcpy(&mt_jmp, mod->bytes + i, sizeof(int32_t));
                i += 4;
                printf("%s %lu\n", mt_op_str(MT_JMP), i + mt_jmp);
                break;
            default:
                printf("\n");
        }
    }
}
