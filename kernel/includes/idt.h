#ifndef IDT_H
#define IDT_H

#include <stdint.h>
#include <string.h>

#include "isr.h"
#include "irq.h"
#include "pic.h"

#define IDT_SIZE 256

void idt_init();

void exception_handler(registers_t* registers);

void exception_handler_no_err(registers_t* registers);

void print_all_registers_t(registers_t *registers);

void generic_fault(registers_t* regs);

uint32_t get_current_cr3();

#endif
