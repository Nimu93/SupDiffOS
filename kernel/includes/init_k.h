#ifndef INIT_K_H
#define INIT_K_H

#include <stddef.h>
#include <stdio.h>
#include "terminal.h"
#include "asm_io.h"
#include "idt.h"
#include "memory.h"
#include "gdt.h"
#include "timer.h"

int init_drivers();

void print_welcome_message();

#endif