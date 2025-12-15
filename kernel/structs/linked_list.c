#include "linked_list.h"

linked_list_t* ll_init(void) {
    linked_list_t* list = (linked_list_t*)kmalloc(sizeof(linked_list_t));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void ll_insert(linked_list_t* list, void* data) {
    linked_list_node_t* new_node = (linked_list_node_t*)kmalloc(sizeof(linked_list_node_t));
    new_node->data = data;
    new_node->next = NULL;

    if (list->tail == NULL) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

void ll_remove(linked_list_t* list, void* data) {
    linked_list_node_t* current = list->head;
    linked_list_node_t* previous = NULL;

    while (current != NULL) {
        if (current->data == data) {
            if (previous == NULL) {
                list->head = current->next;
            } else {
                previous->next = current->next;
            }
            if (current == list->tail) {
                list->tail = previous;
            }
            kfree(current);
            list->size--;
            return;
        }
        previous = current;
        current = current->next;
    }
}

void ll_clear(linked_list_t* list) {
    linked_list_node_t* current = list->head;
    while (current != NULL) {
        linked_list_node_t* next = current->next;
        kfree(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void ll_destroy(linked_list_t* list) {
    ll_clear(list);
    kfree(list);
}

void ll_print(linked_list_t* list, void (*print_func)(void*)) {
    linked_list_node_t* current = list->head;
    while (current != NULL) {
        print_func(current->data);
        current = current->next;
    }
}
