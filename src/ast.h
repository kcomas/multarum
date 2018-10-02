
#ifndef MT_AST_H
#define MT_AST_H

#include "hash.h"

#define mt_ast(NAME) mt_pfx(_A_##NAME)

typedef struct _mt_ast mt_ast;

typedef struct {
    struct {
        uint64_t idx;
        mt_hash hash;
    } args_table, var_table;
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

typedef enum {
    mt_ast(ASSIGN),
    mt_ast(FN)
} mt_ast_type;

typedef struct _mt_ast {
    mt_ast_type type;
    mt_ast_node node;
} mt_ast;

mt_ast* mt_ast_init(size_t arg_size, size_t local_size);

#endif
