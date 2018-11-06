
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
#   define MT_AST_LOCAL_SIZE 20
#endif

#ifndef MT_AST_FN_SIZE
#   define MT_AST_FN_SIZE 30
#endif

typedef struct {
    struct {
        uint64_t idx;
        mt_hash* hash;
    } arg_table, local_table, fn_table;
} mt_ast_sym_table;

typedef struct _mt_ast_op_list {
    mt_ast* op;
    struct _mt_ast_op_list* next;
} mt_ast_op_list;

typedef struct {
    mt_ast_sym_table sym_table;
    mt_ast_op_list* ops_head;
    mt_ast_op_list* ops_tail;
} mt_ast_fn;

#define mt_ast_fn_access(fn, target) fn->sym_table.target

typedef struct _mt_ast_if_cond {
    mt_ast* cond;
    mt_ast* body;
    struct _mt_ast_if_cond* next;
} mt_ast_if_cond;

typedef struct {
    mt_ast_if_cond* conds;
    mt_ast* def;
} mt_ast_if;

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
    mt_ast(NULL),
    mt_ast(FN),
    mt_ast(ASSIGN),
    mt_ast(VAR),
    mt_ast(ARG),
    mt_ast(INT),
    // mt_ast(FLOAT)
} mt_ast_type;

typedef struct _mt_ast {
    mt_ast_type type;
    mt_ast_node node;
} mt_ast;

#define mt_ast_state(NAME) mt_pfx(_S_M_##NAME)

typedef enum {
    mt_ast_state(MAIN),
    mt_ast_state(FN),
    mt_ast_state(ARGS),
    mt_ast_state(IF),
} mt_ast_state_mode;

typedef struct {
    mt_ast_state_mode mode;
    mt_token* cur_token;
    mt_ast* ast;
} mt_ast_state;

void mt_ast_init(mt_ast_state* const state);

mt_var mt_ast_build(mt_ast_state* const state, mt_token* const tokens);

void mt_ast_debug_print(const mt_ast* const ast, uint32_t indent);

#endif
