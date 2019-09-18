
#pragma once

#include <stdint.h>
#include "token.h"

#define AST_VAR(NAME) AST_VAR_##NAME

typedef enum {
    AST_VAR(UNKNOWN)
} ast_var_ident;

typedef struct {

} ast_var_type;

typedef struct _ast_node ast_node;

typedef struct {
    ast_var_type type;
    size_t len;
    char name[];
} ast_var;

typedef struct _symtbl_node {
    ast_var *var;
    struct _symtbl_node *next;
} symtbl_node;

typedef struct {
    size_t len, size;
    symtbl_node *buckets[];
} symtbl;

typedef struct {
    symtbl *tbl;
} ast_block;

#define AST_NODE(NAME) AST_NODE_##NAME

typedef enum {
    AST_NODE(MODULE)
} ast_type;

typedef struct _ast_node {
    ast_type type;
    union {
        struct {

        } module;
    } data;
} ast_node;

#define PARSER_NODE(NAME) PARSER_NODE_##NAME

typedef enum {
    PARSER_NODE(TOKEN),
    PARSER_NODE(NODE),
    PARSER_NODE(BRANCH)
} parser_node_type;

typedef struct _parser_node parser_node;

typedef struct {
    size_t len;
    parser_node *parts[];
} production;

typedef struct {
    size_t len;
    parser_node *branches[];
} branch;

typedef struct _parser_node {
    parser_node_type type;
    union {
        token_type tt;
        production *p;
        branch *b;
    } data;
} parser_node;

#define PARSER_LIST(NAME) PARSER_LIST_##NAME

typedef enum {
    PARSER_LIST(TOKEN),
    PARSER_LIST(AST)
} parser_list_type;

typedef struct _parser_list {
    parser_list_type type;
    union {
        token *t;
        ast_node *a;
    } data;
} parser_list;

typedef struct {
    size_t len;
    struct {
        parser_list *list;
        parser_node *selected;
    }[] stack;
} parser_state;
