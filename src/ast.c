
#include "ast.h"

static inline mt_ast make_node(mt_ast_type type, mt_ast_node node) {
    return (mt_ast) { .type = type, .node = node };
}

mt_ast* mt_ast_init(size_t arg_size, size_t local_size) {

}
