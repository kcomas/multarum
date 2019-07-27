
#pragma once

#include "token.h"

typedef struct _ast_node {

} ast_node;

typedef struct _parser_node parser_node;

#define MATCHCHAIN(NAME) MATCHCHAIN_##NAME

typedef struct {
    enum {
        MATCHCHAIN(TOKEN),
        MATCHCHAIN(PARSER_NODE)
    } type;
    union {
        token_type tt;
        parser_node *ps;
    } value;
} match_chain_item;

#define PARSER(NAME) PARSER_##NAME

typedef struct _parser_node {
    enum {
    } type;
    match_chain_items item[];
} parser_node;
