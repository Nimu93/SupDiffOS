#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>
#include <stdint.h>
#include "kheap.h"

typedef struct linked_list_node {
    void *data;
    struct linked_list_node *next;
} linked_list_node_t;

typedef struct linked_list {
    linked_list_node_t *head;
    linked_list_node_t *tail;
    size_t size;
} linked_list_t;

linked_list_t* ll_init();

void ll_insert(linked_list_t* list, void* data);

void ll_remove(linked_list_t* list, void* data);

void ll_destroy(linked_list_t* list);

void ll_clear(linked_list_t* list);

void ll_print(linked_list_t* list, void (*print_func)(void*));


#endif
