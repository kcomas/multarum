
#ifndef MT_AST_H
#define MT_AST_H

#include "token.h"
#include "hash.h"

#define mt_ast(NAME) mt_pfx(_A_##NAME)

typedef struct _mt_ast mt_ast;

#ifndef MT_AST_ARG_SIZE
#   define MT_AST_ARG_SIZE 10
#endif

#ifndef MT_AST_LOCAL_SIZE
#   define MT_AST_LOCAL_SIZE 30
#endif

typedef struct {
    struct {
        uint64_t idx;
        mt_hash* hash;
    } args_table, local_table;
} mt_sym_table;

typedef struct _mt_ast_op_list {
    mt_ast* op;
    struct _mt_ast_op_list* next;
} mt_ast_op_list;

typedef struct {
    mt_sym_table sym_table;
    mt_ast_op_list* ops;
} mt_ast_fn;

typedef struct {
    mt_ast* left;
    mt_ast* right;
} mt_ast_bop;

typedef union {
    int64_t mt_int;
    double mt_float;
    mt_buf* mt_var;
} mt_ast_value;

typedef union {
    mt_ast_value value;
    mt_ast_fn* fn;
    mt_ast_bop* bop;
} mt_ast_node;

typedef enum {
    mt_ast(FN),
    mt_ast(ASSIGN),
    mt_ast(VAR),
    mt_ast(INT),
    // mt_ast(FLOAT)
} mt_ast_type;

typedef struct _mt_ast {
    mt_ast_type type;
    mt_ast_node node;
} mt_ast;

typedef struct {
    mt_token* cur_token;
    mt_ast* ast;
    mt_ast* last;
} mt_ast_state;

void mt_ast_init(mt_ast_state* const state);

mt_var mt_ast_build(mt_ast_state* const state, mt_token* const tokens);

#endif
