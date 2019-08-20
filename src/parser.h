
#pragma once

#include <stdint.h>
#include "token.h"

#define AST_NODE(NAME) AST_NODE_##NAME

typedef enum {

} ast_type;

typedef struct _ast_node {
    ast_type type;
    union {

    } data;
} ast_node;

#define PARSER_NODE(NAME) PARSER_NODE_##NAME

typedef enum {
    PARSER_NODE(MATCH),
    PARSER_NODE(NODE)
} parser_node_type;

typedef struct _parser_node {
    parser_node_type type;
    union {
        token_type token_type;
        struct {

        } node;
    } date;
} parser_node;

typedef struct _token_list {
    token t;
    struct _token_list *next;
} token_list;

typedef struct {

} parser_state;
