
#include "mod.h"

mt_mod* mt_mod_init(size_t _size, size_t _f_size) {
    mt_mod* mod = (mt_mod*) malloc(sizeof(mt_mod));
    mod->ref_count = 1;
    mod->_size = _size;
    mod->len = 0;
    mod->bytes = (uint8_t*) malloc(_size);
    mod->_f_size = _f_size;
    mod->f_len = 0;
    mod->fns = (uint8_t*) malloc(_f_size);
    return mod;
}

void mt_mod_free(mt_mod* const mod) {
    mod->ref_count--;
    if (mod->ref_count > 0) {
        return;
    }
    free(mod->bytes);
    free(mod->fns);
    free(mod);
}

static void mt_print_byte_hex(const mt_mod* const mod, size_t i, size_t total) {
    for (size_t x = i; x < i + total; x++) {
        printf("%02x ", mod->bytes[x]);
    }
}

static size_t mt_mod_num_len(size_t i) {
    if (i == 0) {
        return 1;
    }
    size_t count = 0;
    while (i >= 1) {
        i /= 10;
        count++;
    }
    return count;
}

static void mt_mod_print_even_spaces(size_t cur_len) {
    while (cur_len < MT_MOD_MAX_BYTES_LINE) {
        for (size_t x = 0; x < MT_MOD_BYTE_PRINT_LINE; x++) {
            putchar(' ');
        }
        cur_len++;
    }
}

void mt_mod_dis(const mt_mod* const mod) {
    mt_op_str_init();

    uint32_t mt_jmp;
    int64_t mt_int;
    double mt_float;

    size_t i = 0;
    size_t count_total = mt_mod_num_len(mod->len);
    while (i < mod->len) {
        for (size_t fi = 0; fi < mod->f_len; fi++) {
            if (mod->fns[fi] == i) {
                printf("FN: %lu\n", fi);
                break;
            }
        }
        printf("%lu: ", i);
        for (size_t count = mt_mod_num_len(i); count < count_total; count++) {
            putchar(' ');
        }
        switch (mod->bytes[i]) {
            case MT_NOP:
            case MT_ADD:
            case MT_SUB:
            case MT_LD_SELF:
            case MT_RET:
            case MT_HALT:
                mt_print_byte_hex(mod, i, 1);
                mt_mod_print_even_spaces(1);
                printf("%s\n", mt_op_str(mod->bytes[i]));
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
                mt_mod_copy_inc(mod, i, mt_jmp, uint32_t);
                mt_mod_print_even_spaces(1 + sizeof(uint32_t));
                printf("JMP %d\n", mt_jmp);
                break;
            default:
                i++;
                printf("\n");
                break;
        }
    }
}
