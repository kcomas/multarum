
#include "var.h"

void mt_var_write_bytes(mt_mod* const mod, const mt_var* const var) {
    mt_write_byte(mod, var->type);
    switch (var->type) {
        case MT_NULL:
            break;
        case MT_BOOL:
            mt_write_byte(mod, var->data.mt_bool);
            break;
        case MT_CHAR:
            mt_write_byte(mod, var->data.mt_char);
            break;
        case MT_INT:
            mt_write_bytes(mod, &var->data.mt_int, sizeof(int64_t));
            break;
        case MT_FLOAT:
            mt_write_bytes(mod, &var->data.mt_float, sizeof(double));
            break;
        default:
            break;
        }
}

void mt_var_debug_print(const mt_var* const var) {
    switch (var->type) {
        case MT_NULL:
            printf("NULL");
            break;
        case MT_BOOL:
            printf("%s", var->data.mt_bool ? "t" : "f");
            break;
        case MT_CHAR:
            printf("%c", var->data.mt_char);
            break;
        case MT_INT:
            printf("%li", var->data.mt_int);
            break;
        case MT_FLOAT:
            printf("%lf", var->data.mt_float);
            break;
        case MT_MODULE:
            printf("Mod<%p>", var->data.mt_mod);
            break;
        case MT_FN:
            printf("Fn %d Mod<%p>", var->fn_idx, var->data.mt_mod);
            break;
    }
}
