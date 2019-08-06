
#pragma once

#include <stdint.h>
#include "token.h"

#define AST_VAR(NAME) AST_VAR_##NAME

typedef enum {
    AST_VAR(UNKNOWN),
    AST_VAR(FN),
    AST_VAR(INT),
    AST_VAR(FLOAT)
} ast_var_type_ident;

typedef struct {
    ast_var_type_ident ident;
} ast_var_type;

typedef struct {
    ast_var_type type;
    size_t len;
    char value[];
} ast_var;

ast_var *ast_var_init(ast_var_type type, size_t len, char *value);

#define MAKE_LIST(NAME, TYPE) \
    typedef struct _##NAME { \
        TYPE *value; \
        struct _##NAME *next; \
    } NAME

MAKE_LIST(hash_symtbl_node, ast_var);

typedef struct {
    size_t size, usued;
    hash_symtbl_node *buckets[];
} hash_symtbl;

hash_symtbl *hash_symtbl_init(size_t size);

#define AST(NAME) AST_##NAME

typedef enum {
    AST(FN),
    AST(IF),
    AST(EXPR),
    AST(VAR),
    AST(INTEGER)
} ast_node_type;

typedef struct _ast_node ast_node;

MAKE_LIST(ast_node_list, ast_node);

typedef struct {
    hash_symtbl *locals;
    ast_node *parent;
    ast_node_list *body;
} ast_block;

MAKE_LIST(ast_block_list, ast_block);

typedef struct {
    ast_node *cond;
    ast_block *body;
} ast_cond;

MAKE_LIST(ast_cond_list, ast_cond);

typedef struct _ast_node {
    ast_node_type type;
    union {
        struct {
            ast_var_type return_type;
            hash_symtbl *args;
            ast_block *body;
        } fn;
        struct {
            ast_cond *if_main;
            ast_cond_list *if_elif;
            ast_cond *if_else;
        } if_cond;
        ast_node *expr;
        ast_var var;
        int64_t integer;
    } value;
} ast_node;

typedef struct {
    ast_var_type export_type;
    ast_block *body;
    ast_node_list *fns;
} ast_module;
