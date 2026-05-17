#ifndef INIT_K_H
#define INIT_K_H

#include <stddef.h>
#include <stdio.h>
#include "drivers/terminal.h"
#include "arch/x86/asm_io.h"
#include "arch/x86/idt.h"
#include "mm/memory.h"
#include "arch/x86/gdt.h"
#include "arch/x86/timer.h"

int init_drivers();

void print_welcome_message();

#endif