
#include "mod.h"

mt_mod* mt_mod_init(uint16_t _size, uint8_t _f_size) {
    mt_mod* mod = (mt_mod*) malloc(sizeof(mt_mod));
    mod->ref_count = 1;
    mod->_size = _size;
    mod->len = 0;
    mod->bytes = (uint8_t*) malloc(_size);
    mod->_f_size = _f_size;
    mod->f_len = 0;
    mod->fns = (uint16_t*) malloc(sizeof(uint16_t) * _f_size);
    mod->fne = (uint16_t*) malloc(sizeof(uint16_t) * _f_size);
    mod->globals = mt_hash_init(MT_MOD_GLOBAL_BUCKETS);
    return mod;
}

void mt_mod_free(mt_mod* const mod) {
    if ((--mod->ref_count) > 0) {
        return;
    }
    free(mod->bytes);
    free(mod->fns);
    free(mod->fne);
    mt_hash_free(mod->globals);
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

static size_t mt_mod_op16(const mt_mod* const mod, size_t i) {
    uint16_t mt_al = 0;
    uint8_t op = mod->bytes[i];
    i = mt_mod_op_w_data(mod, i, sizeof(uint16_t), &mt_al);
    printf("%s %d\n", mt_op_str(op), mt_al);
    return i;
}

static inline void mt_mod_space_bytes(size_t i, size_t count_total) {
    for (size_t count = mt_mod_num_len(i); count < count_total; count++) {
        putchar(' ');
    }
}

static size_t mt_mod_single_byte_cmd(const mt_mod* mod, size_t i) {
    mt_print_byte_hex(mod, i, 1);
    mt_mod_print_even_spaces(1);
    printf("%s\n", mt_op_str(mod->bytes[i]));
    return i + 1;
}

static size_t mt_print_next_op(const mt_mod* const mod, size_t i, size_t count_total) {
    int8_t char_conts;
    uint32_t mt_jmp, str_len;
    int64_t mt_int;
    double mt_float;
    mt_char mt_char_parts = mt_char_init(0, 0, 0, 0);

    printf("%lu: ", i);
    mt_mod_space_bytes(i, count_total);
    switch (mod->bytes[i]) {
        case mt_pfx(AL):
            i = mt_mod_op16(mod, i);
            break;
        case mt_pfx(NOP):
        case mt_pfx(MUL):
        case mt_pfx(ADD):
        case mt_pfx(SUB):
        case mt_pfx(EQ):
        case mt_pfx(OR):
        case mt_pfx(LD_SELF):
        case mt_pfx(RET):
        case mt_pfx(WRITE):
        case mt_pfx(HALT):
            i = mt_mod_single_byte_cmd(mod, i);
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
                    mt_char_parts.a = mod->bytes[++i];
                    char_conts = mt_char_cont(mt_char_parts.a);
                    mt_print_byte_hex(mod, i + 1, char_conts);
                    if (char_conts < 0) {
                        mt_mod_print_even_spaces(3 + char_conts);
                        printf("Invalid utf8 char\n");
                        i++;
                        break;
                    }
                    if (char_conts >= 1) {
                        mt_char_parts.b = mod->bytes[++i];
                        if (char_conts >= 2) {
                            mt_char_parts.c = mod->bytes[++i];
                            if (char_conts == 3) {
                                mt_char_parts.d = mod->bytes[++i];
                            }
                        }
                    }
                    mt_mod_print_even_spaces(3 + char_conts);
                    printf("PUSH ");
                    putchar(mt_char_parts.a);
                    if (char_conts >= 1) {
                        putchar(mt_char_parts.b);
                        if (char_conts >= 2) {
                            putchar(mt_char_parts.c);
                            if (mt_char_parts.c) {
                                if (mt_char_parts.d) {
                                    putchar(mt_char_parts.d);
                                }
                            }
                        }
                    }
                    putchar('\n');
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
        case mt_pfx(ISTR):
            i = mt_mod_op_w_data(mod, i, sizeof(uint32_t), &str_len);
            printf("ISTR %d ", str_len);
            for (size_t x = 0; x < str_len; x++, i++) {
                switch (mod->bytes[i]) {
                    case '\\':
                        printf("\\\\");
                        break;
                    case '\t':
                        printf("\\t");
                        break;
                    case '\n':
                        printf("\\n");
                        break;
                    default:
                        putchar(mod->bytes[i]);
                        break;
                }
            }
            printf("\n");
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
        case mt_pfx(SV_ARG):
        case mt_pfx(CALL_SELF):
        case mt_pfx(CALL):
            i = mt_mod_op2(mod, i);
            break;
        case mt_pfx(LD_LOCAL):
        case mt_pfx(SV_LOCAL):
            i = mt_mod_op16(mod, i);
            break;
        default:
            i++;
            printf("\n");
            break;
    }
    return i;
}

#define mt_mod_show_fn(mod, target, msg) \
    for (size_t fi = 0; fi < mod->f_len; fi++) { \
        if (mod->target[fi] == i) { \
            printf("%s: %lu\n", msg, fi); \
            break; \
        } \
    }

void mt_mod_dis(const mt_mod* const mod) {
    size_t i = 0;
    size_t count_total = mt_mod_num_len(mod->len);
    while (i < mod->len) {
        mt_mod_show_fn(mod, fns, "FN");
        i = mt_print_next_op(mod, i, count_total);
        mt_mod_show_fn(mod, fne, "END FN");
    }
}
