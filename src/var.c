
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
        }
}
