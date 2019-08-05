
#include "parser.h"

hash_symtbl *hash_symtbl_init(size_t size) {
    hash_symtbl *h = calloc(1, sizeof(hash_symtbl) + sizeof(hash_symtbl_node*) * size);
    h->size = size;
    return h;
}

#define LIST_NODE_INIT(TYPE, VALUE_TYPE) \
    static inline TYPE *TYPE##_init(VALUE_TYPE *value) { \
        TYPE *node = calloc(1, sizeof(TYPE)); \
        node->value = value; \
        return node; \
    }

LIST_NODE_INIT(hash_symtbl_node, ast_var);
