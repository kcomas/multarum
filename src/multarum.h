
#ifndef MULTARUM_H
#define MULTARUM_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define mt_pfx(NAME) MT_##NAME

typedef struct _mt_mod mt_mod;
typedef struct _mt_var mt_var;

#include "var.h"
#include "op.h"
#include "mod.h"
#include "vm.h"
#include "err.h"

#endif
