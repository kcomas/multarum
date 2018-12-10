
#ifndef MT_COMMON_H
#define MT_COMMON_H

#define MT_VERSION "0.0.0"

#define mt_pfx(NAME) MT_##NAME

#define MT_TYPES \
    mt_pfx(NULL), \
    mt_pfx(BOOL), \
    mt_pfx(CHAR), \
    mt_pfx(INT), \
    mt_pfx(FLOAT), \
    mt_pfx(MODULE), \
    mt_pfx(FN), \
    mt_pfx(MFILE), \
    mt_pfx(ERROR), \
    mt_pfx(BUFFER), \
    mt_pfx(HASH), \
    mt_pfx(STR)

#endif
