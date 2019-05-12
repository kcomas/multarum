
#pragma once

#include <stdlib.h>
#include <string.h>
#include "utf.h"

typedef struct _str {
    size_t size, len;
    char data[];
} *str;

str str_init(size_t size);

str str_from_cstring(char *cstring);
