#include "irq.h"

void enable_interrupts() {
    __asm__ volatile ("sti");
}

void disable_interrupts() {
    __asm__ volatile ("cli");
}

void keyboard_handler(registers_t* regs) {
    uint8_t scancode = inb(0x60);  // read from keyboard data port
    get_keyboard(scancode);
}


void timer_handler(registers_t* regs) {

    // Toujours envoyer un EOI (End of Interrupt) avant de potentiellement basculer
    outb(0x20, 0x20);
    handle_timer();
}
