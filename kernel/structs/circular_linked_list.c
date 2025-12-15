#include "circular_linked_list.h"


struct circular_linked_list *init_circular_linked_list(void) {
    struct circular_linked_list *list = (struct circular_linked_list *)kmalloc(sizeof(struct circular_linked_list));
    list->tail = NULL;
    return list;
}

void cll_insert(struct circular_linked_list *list, void *data) {
    struct CLLNode *node = (struct CLLNode *)kmalloc(sizeof(struct CLLNode));
    node->data = data;

    if (list->tail == NULL) {
        node->next = node; 
        list->tail = node;
        return;
    }

    node->next = list->tail->next; 
    list->tail->next = node;
    list->tail = node;
}

void *cll_remove(struct circular_linked_list *list) {
    if (list->tail == NULL) {
        return NULL;
    }

    struct CLLNode *head = list->tail->next;
    void *data = head->data;

    if (head == list->tail) {
        list->tail = NULL;
    } else {
        list->tail->next = head->next;
    }

    kfree(head);
    return data;
}

void *cll_peek(struct circular_linked_list *list) {
    return (list->tail == NULL) ? NULL : list->tail->next->data;
}

int cll_is_empty(struct circular_linked_list *list) {
    return list->tail == NULL;
}

void cll_clear(struct circular_linked_list *list) {
    while (!cll_is_empty(list)) {
        cll_remove(list);
    }
    kfree(list);
}
