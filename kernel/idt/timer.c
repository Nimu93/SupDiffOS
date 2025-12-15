#include "timer.h"


static uint64_t tick = 0;
static uint64_t seconds = 0;

static linked_list_t* timer_list = NULL;

void init_timer() {
    tick = 0;
    seconds = 0;
    timer_list = ll_init();
}

void handle_timer() {
    tick++;
    if (tick % 100 == 0) {
        seconds++;
    }
    linked_list_node_t* current = timer_list->head;
    while (current != NULL) {
        void (*callback)(void) = (void (*)(void))current->data;
        if (callback != NULL) {
            callback();
        }
        current = current->next;
    }
}

void insert_tick_action(void *action) {
    ll_insert(timer_list, action);
}

uint64_t read_current_tick() {
    return tick;
}
