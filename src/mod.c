
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
