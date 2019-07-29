
#pragma once

#include "token.h"

#define AST(NAME) AST_##NAME

typedef enum {

} ast_node_type;

typedef struct _ast_node {

} ast_node;

typedef struct _parser_node parser_node;

#define MATCHCHAIN(NAME) MATCHCHAIN_##NAME

typedef enum {
    MATCHCHAIN(TOKEN),
    MATCHCHAIN(PARSER_NODE)
} match_chain_type;

typedef struct {
    match_chain_type type;
    union {
        token_type tt;
        parser_node *ps;
    } value;
} match_chain_item;

#define PARSER(NAME) PARSER_##NAME

typedef enum {

} parser_node_type;

typedef struct _parser_node {
    match_chain_items item[];
} parser_node;

typedef struct {

} parser_state;
