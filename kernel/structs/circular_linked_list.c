#include "circular_linked_list.h"

static struct CLLNode *cll_tail = NULL;

void init_circular_linked_list(void) {
    cll_tail = NULL;
}

void cll_insert(void *data) {
    struct CLLNode *node = (struct CLLNode *)kmalloc(sizeof(struct CLLNode));
    node->data = data;
    if (cll_tail == NULL) {
        node->next = node;  
        cll_tail = node;
    } else {
        node->next = cll_tail->next; 
        cll_tail->next = node;
        cll_tail = node;        
    }
}

void *cll_remove(void) {
    if (cll_tail == NULL) {
        return NULL;
    }

    struct CLLNode *head = cll_tail->next;
    void *data = head->data;

    if (head == cll_tail) {
        cll_tail = NULL;
    } else {
        cll_tail->next = head->next; 
    }

    kfree(head);
    return data;
}

void *cll_peek(void) {
    if (cll_tail == NULL) {
        return NULL;
    }
    return cll_tail->next->data;
}

int cll_is_empty(void) {
    return cll_tail == NULL;
}

void cll_clear(void) {
    while (!cll_is_empty()) {
        cll_remove();
    }
}
