#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include "arch/x86/data_idt.h"

// Syscall numbers
#define SYS_EXIT    0
#define SYS_WRITE   1
#define SYS_READ    2
#define SYS_GETPID  3

// Number of syscalls
#define NUM_SYSCALLS 4

// Initialize syscall handler (register INT 0x80)
void syscall_init(void);

// Syscall handler called from assembly
void syscall_handler(registers_t *regs);

#endif // SYSCALL_H
