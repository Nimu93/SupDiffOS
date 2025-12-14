#include "terminal.h"
#include "init_k.h"
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "memory.h"
#include "multitasking.h"

void kernel_main(void) 
{
	init_drivers();
	printf("Kernel initialized successfully!\n");
	init_multitasking();
	for(;;);
}