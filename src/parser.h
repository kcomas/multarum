
#pragma once

#include "token.h"

#define AST(NAME) AST_##NAME

typedef enum {
    AST(FN),
} ast_node_type;

typedef struct _ast_node {
    ast_node_type type;
    union {

    } value;
} ast_node;

typedef struct {

} ast_module;
