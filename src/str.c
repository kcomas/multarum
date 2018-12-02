
#include "str.h"

static inline mt_str_link* mt_str_add_node(mt_buf* const buf) {
    mt_str_link* link = malloc(sizeof(mt_str_link));
    link->data = buf;
    link->next = NULL;
    return link;
}

mt_str* mt_str_init(mt_buf* const buf) {
    mt_str* str = malloc(sizeof(mt_str));
    str->ref_count = 1;
    str->len = buf->len;
    str->_chunks = 1;
    str->head = mt_str_add_node(buf);
    str->tail = str->head;
    return str;
}

void mt_str_free(mt_str* const str) {
    if (--str->ref_count > 0) {
        return;
    }
    mt_str_link* head = str->head;
    while (head != NULL) {
        mt_str_link* tmp = head;
        head = head->next;
        mt_buf_free(tmp->data);
        free(tmp);
    }
    free(str);
}

void mt_str_debug_print(const mt_str* const str) {
    mt_str_link* head = str->head;
    while (head != NULL) {
        mt_buf_debug_print(head->data);
        head = head->next;
    }
}
