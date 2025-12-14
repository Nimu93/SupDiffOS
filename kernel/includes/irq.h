#ifndef IRQ_H
#define IRQ_H

#include "idt.h"
#include "keyboard.h"

void enable_interrupts();

void disable_interrupts();

void keyboard_handler(registers_t* regs);

void timer_handler(registers_t* regs);

#endif //IRQ_H