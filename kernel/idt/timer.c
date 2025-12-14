#include "timer.h"


static uint64_t tick = 0;
static uint64_t seconds = 0;
void handle_timer() {
    tick++;
    if (tick % 100 == 0) {
        seconds++;
    }
}

uint64_t read_current_tick() {
    return tick;
}