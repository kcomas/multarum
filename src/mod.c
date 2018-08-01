
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

static size_t mt_mod_op_w_data(const mt_mod* const mod, size_t i, size_t size, void* const data) {
    mt_print_byte_hex(mod, i, 1 + size);
    i++;
    memcpy(data, mod->bytes + i, size);
    i += size;
    mt_mod_print_even_spaces(1 + size);
    return i;
}

static size_t mt_mod_op2(const mt_mod* const mod, size_t i) {
    mt_print_byte_hex(mod, i, 2);
    mt_mod_print_even_spaces(2);
    printf("%s %d\n", mt_op_str(mod->bytes[i]), mod->bytes[i + 1]);
    return i + 2;
}

void mt_mod_dis(const mt_mod* const mod) {
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
            case mt_pfx(NOP):
            case mt_pfx(ADD):
            case mt_pfx(SUB):
            case mt_pfx(EQ):
            case mt_pfx(LD_SELF):
            case mt_pfx(RET):
            case mt_pfx(HALT):
                mt_print_byte_hex(mod, i, 1);
                mt_mod_print_even_spaces(1);
                printf("%s\n", mt_op_str(mod->bytes[i]));
                i++;
                break;
            case mt_pfx(PUSH):
                mt_print_byte_hex(mod, i, 1);
                switch (mod->bytes[++i]) {
                    case mt_pfx(NULL):
                        mt_print_byte_hex(mod, i, 1);
                        mt_mod_print_even_spaces(2);
                        printf("PUSH NULL\n");
                        i++;
                        break;
                    case mt_pfx(BOOL):
                        mt_print_byte_hex(mod, i, 2);
                        mt_mod_print_even_spaces(3);
                        printf("PUSH %s\n", mod->bytes[++i] == true ? "true" : "false");
                        i++;
                        break;
                    case mt_pfx(CHAR):
                        mt_print_byte_hex(mod, i, 2);
                        mt_mod_print_even_spaces(3);
                        printf("PUSH %c\n", mod->bytes[++i]);
                        i++;
                        break;
                    case mt_pfx(INT):
                        mt_print_byte_hex(mod, i, 9);
                        mt_mod_get_bytes(mod, &mt_int, sizeof(int64_t), i);
                        printf("PUSH %li\n", mt_int);
                        break;
                    case mt_pfx(FLOAT):
                        mt_print_byte_hex(mod, i, 9);
                        mt_mod_get_bytes(mod, &mt_float, sizeof(double), i);
                        printf("PUSH %lf\n", mt_float);
                        break;
                }
                break;
            case mt_pfx(JMP):
                i = mt_mod_op_w_data(mod, i, sizeof(uint32_t), &mt_jmp);
                printf("JMP %d\n", mt_jmp);
                break;
            case mt_pfx(JMPF):
                i = mt_mod_op_w_data(mod, i, sizeof(uint32_t), &mt_jmp);
                printf("JMPF %d\n", mt_jmp);
                break;
            case mt_pfx(LD_FN):
            case mt_pfx(LD_ARG):
            case mt_pfx(CALL_SELF):
            case mt_pfx(CALL):
                i = mt_mod_op2(mod, i);
                break;
            default:
                i++;
                printf("\n");
                break;
        }
    }
}
