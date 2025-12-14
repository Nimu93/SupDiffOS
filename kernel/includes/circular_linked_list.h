#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

#include "kheap.h"


struct CLLNode {
    void* data;
    struct CLLNode* next;
};

void init_circular_linked_list();
void cll_insert(void* data);
void* cll_remove();
void* cll_peek();
int cll_is_empty();
void cll_clear();

#endif // CIRCULAR_LINKED_LIST_H