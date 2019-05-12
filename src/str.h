
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utf.h"

typedef struct _str {
    size_t size, len;
    char data[];
} *str;

str str_init(size_t size);

void str_free(str s);

str str_from_cstring(char *cstring);
