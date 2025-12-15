#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "linked_list.h"
#include "asm_io.h"

void init_timer();

void handle_timer();

uint64_t read_current_tick();

void insert_tick_action(void *action);

#endif // TIMER_H
