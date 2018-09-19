
#ifndef MT_AST_H
#define MT_AST_H

#include "common.h"
#include "buf.h"

#define mt_ast(NAME) mt_pfx(_A_##NAME)

typedef struct _mt_ast mt_ast;

typedef enum {
    mt_ast(ASSIGN),
    mt_ast(FN)
} mt_ast_type;

typedef struct {

} mt_sym_table;

typedef struct {
    mt_ast* op;
    mt_ast* next;
} mt_ast_op_list;

typedef struct {
    mt_sym_table* sym_table;
    mt_ast_op_list* ops;
} mt_ast_fn;

typedef struct {
    mt_ast* left;
    mt_ast* right;
} mt_ast_bop;

typedef union {
    mt_ast_fn* fn;
    mt_ast_bop* bop;
} mt_ast_node;

typedef struct _mt_ast {
    mt_ast_type type;
    mt_ast_node node;
} mt_ast;

#endif
