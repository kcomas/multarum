
#ifndef MT_AST_H
#define MT_AST_H

#include "common.h"
#include "buf.h"

#define mt_ast(NAME) mt_pfx(_A_##NAME)

typedef enum {
    mt_ast(COND),
    mt_ast(IF_COND),
    mt_ast(IF)
} mt_ast_type;

typedef struct _mt_ast mt_ast;

typedef union {
    struct {
        // @TODO cond type
        mt_ast* left;
        mt_ast* right;
    } mt_ast_cond;
    struct {
        mt_ast* cmp;
        mt_ast* next;
    } mt_ast_if_cond;
    struct {
        mt_ast* cond;
        mt_ast* def;
    } mt_ast_if;
} mt_ast_node;

typedef struct _mt_ast {
    mt_ast_type type;
    mt_ast_node node;
} mt_ast;

#endif
