
#pragma once

#include "token.h"

typedef struct _parser_node parser_node;

#define PARSER(NAME) PARSER_##NAME

typedef struct {
    enum {
        PARSER(TOKEN),
        PARSER(PARSER_NODE)
    } type;
    union {
        token_type tt;
        parser_node *ps;
    } value;
} match_chain_item;

typedef struct _parser_node {
    match_chain chain[];
} parser_node;
