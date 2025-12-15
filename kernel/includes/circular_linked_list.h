#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include "kheap.h"

struct CLLNode {
    void* data;
    struct CLLNode* next;
};

struct circular_linked_list {
    struct CLLNode* tail;  /* tail->next is the head */
};

struct circular_linked_list* init_circular_linked_list(void);
void cll_insert(struct circular_linked_list* list, void* data);
void* cll_remove(struct circular_linked_list* list);
void* cll_peek(struct circular_linked_list* list);
int cll_is_empty(struct circular_linked_list* list);
void cll_clear(struct circular_linked_list* list);

#endif // CIRCULAR_LINKED_LIST_H
