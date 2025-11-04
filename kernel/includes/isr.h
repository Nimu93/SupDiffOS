#ifndef ISR_H
#define ISR_H
//File used for Interrupt Service Routines

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "memory.h"

#include "data_idt.h"	

#include "idt.h"

void gpf_handler(registers_t* regs);

void page_fault_handler(registers_t* registers);

void double_fault_handler(registers_t* regs);

void invalid_opcode_handler(registers_t* registers);

void control_protection_handler(registers_t* regs);

#endif
