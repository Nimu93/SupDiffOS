#include "irq.h"

void keyboard_handler(registers_t* regs) {
    uint8_t scancode = inb(0x60);  // read from keyboard data port
    get_keyboard(scancode);
}

static uint64_t tick = 0;

void timer_handler(registers_t* regs) {
    tick++;
    if (tick % 100 == 0) {
        printf("Timer tick: %lu\n", tick);
    }

    // Toujours envoyer un EOI (End of Interrupt) au PIC
    outb(0x20, 0x20);
}