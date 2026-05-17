#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <string.h>

#include "arch/x86/isr.h"
#include "arch/x86/irq.h"
#include "arch/x86/pic.h"

#define IDT_SIZE 256

void idt_init();
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);

void exception_handler(registers_t* registers);

void exception_handler_no_err(registers_t* registers);

void print_all_registers_t(registers_t *registers);

void generic_fault(registers_t* regs);

uint32_t get_current_cr3();

#endif
