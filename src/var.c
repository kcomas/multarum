
#include "var.h"

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var) {
    mt_write_byte(mod, var->type);
    switch (var->type) {
        case mt_pfx(NULL):
            break;
        case mt_pfx(BOOL):
            mt_write_byte(mod, var->data.mt_bool);
            break;
        case mt_pfx(CHAR):
            mt_write_byte(mod, var->data.mt_char);
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
            return var->data.mt_char != '\0';
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

void mt_var_debug_print(const mt_var* const var) {
    switch (var->type) {
        case mt_pfx(NULL):
            printf("NULL");
            break;
        case mt_pfx(BOOL):
            printf("%s", var->data.mt_bool ? "#t" : "#f");
            break;
        case mt_pfx(CHAR):
            printf("%c", var->data.mt_char);
            break;
        case mt_pfx(INT):
            printf("%li", var->data.mt_int);
            break;
        case mt_pfx(FLOAT):
            printf("%lf", var->data.mt_float);
            break;
        case mt_pfx(MODULE):
            printf("Mod<%p>", var->data.mt_mod);
            break;
        case mt_pfx(FN):
            printf("Fn %d Mod<%p>", var->meta.fn_idx, var->data.mt_mod);
            break;
        case mt_pfx(MFILE):
            printf("File (fd: %d)", var->data.mt_file);
            break;
        case mt_pfx(ERROR):
            printf("Error, Errno: %d, Msg: %s", var->data.mt_err->no, var->data.mt_err->msg->data);
            break;
        case mt_pfx(BUFFER):
            for (size_t i = 0; i < var->data.mt_buf->len; i++) {
                putchar(var->data.mt_buf->data[i]);
            }
            break;
    }
}
