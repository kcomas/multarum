
#include "var.h"

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var) {
    mt_write_byte(mod, var->type);
    int8_t char_conts;
    switch (var->type) {
        case mt_pfx(NULL):
            break;
        case mt_pfx(BOOL):
            mt_write_byte(mod, var->data.mt_bool);
            break;
        case mt_pfx(CHAR):
            mt_write_byte(mod, var->data.mt_char.a);
            char_conts = mt_char_cont(var->data.mt_char.a);
            if (char_conts < 0) {
                mt_write_byte(mod, '\0');
                break;
            }
            if (char_conts >= 1) {
                mt_write_byte(mod, var->data.mt_char.b);
                if (char_conts >= 2) {
                    mt_write_byte(mod, var->data.mt_char.c);
                    if (char_conts == 3) {
                        mt_write_byte(mod, var->data.mt_char.d);
                    }
                }
            }
            break;
        case mt_pfx(INT):
            mt_write_bytes(mod, &var->data.mt_int, sizeof(int64_t));
            break;
        case mt_pfx(FLOAT):
            mt_write_bytes(mod, &var->data.mt_float, sizeof(double));
            break;
        default:
            break;
        }
}

bool mt_var_as_bool(const mt_var* const var) {
    switch (var->type) {
        case mt_pfx(NULL):
            return false;
        case mt_pfx(BOOL):
            return var->data.mt_bool;
        case mt_pfx(CHAR):
            return var->data.mt_char.a != '\0';
        case mt_pfx(INT):
            return var->data.mt_int != 0;
        case mt_pfx(FLOAT):
            return var->data.mt_float != 0.0;
        case mt_pfx(MODULE):
        case mt_pfx(FN):
        case mt_pfx(MFILE):
        case mt_pfx(ERROR):
        case mt_pfx(BUFFER):
        default:
            return false;
    }
}

void mt_var_debug_print(mt_var const var) {
    int8_t char_conts;
    switch (var.type) {
        case mt_pfx(NULL):
            printf("NULL");
            break;
        case mt_pfx(BOOL):
            printf("#%s", var.data.mt_bool ? "t" : "f");
            break;
        case mt_pfx(CHAR):
            putchar(var.data.mt_char.a);
            char_conts = mt_char_cont(var.data.mt_char.a);
            if (char_conts < 0) {
                printf("Invalid utf8 char\n");
                break;
            }
            if (char_conts >= 1) {
                putchar(var.data.mt_char.b);
                if (char_conts >= 2) {
                    putchar(var.data.mt_char.c);
                    if (char_conts == 3) {
                        putchar(var.data.mt_char.d);
                    }
                }
            }
            break;
        case mt_pfx(INT):
            printf("%li", var.data.mt_int);
            break;
        case mt_pfx(FLOAT):
            printf("%lf", var.data.mt_float);
            break;
        case mt_pfx(MODULE):
            printf("Mod<%p>", var.data.mt_mod);
            break;
        case mt_pfx(FN):
            printf("Fn %d Mod<%p>", var.meta.fn_idx, var.data.mt_mod);
            break;
        case mt_pfx(MFILE):
            printf("File (fd: %d)", var.data.mt_file);
            break;
        case mt_pfx(ERROR):
            printf("Error, Errno: %d, Msg: %s", var.data.mt_err->no, var.data.mt_err->msg->data);
            break;
        case mt_pfx(BUFFER):
            mt_buf_debug_print(var.data.mt_buf);
            break;
        case mt_pfx(HASH):
            mt_hash_debug_print(var.data.mt_hash);
            break;
    }
}
