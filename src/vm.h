
#pragma once

#include "var.h"

#define STACK_SIZE 10000

typedef struct _vm {
    var stack[STACK_SIZE];
} *vm;
