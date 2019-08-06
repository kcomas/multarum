
#include "parser.h"

ast_var *ast_var_init(ast_var_type type, size_t len, char *value) {
    ast_var *v = calloc(1, sizeof(ast_var));
    v->type = type;
    v->len = len;
    memcpy(v->value, value, len);
    return v;
}

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

#define LIST_NODE_INSERT(TYPE, VALUE_TYPE) \
    static inline void TYPE##_insert(TYPE *head, VALUE_TYPE *value) { \
        while (head->next != NULL) head = head->next; \
        head->next = TYPE##_init(value); \
    }

#define LIST_NODE_GET(TYPE, VALUE_TYPE) \
    static inline TYPE *TYPE##_get(TYPE *head, VALUE_TYPE *a, bool (*TYPE##_cmp)(VALUE_TYPE*, VALUE_TYPE*)) { \
        while (head != NULL) { \
            if (TYPE##_cmp(a, head->value)) return head; \
            head = head->next; \
        } \
        return NULL; \
    }

#define LIST_NODE_BUILD(TYPE, VALUE_TYPE) \
    LIST_NODE_INIT(TYPE, VALUE_TYPE); \
    LIST_NODE_INSERT(TYPE, VALUE_TYPE); \
    LIST_NODE_GET(TYPE, VALUE_TYPE);

LIST_NODE_BUILD(hash_symtbl_node, ast_var);
