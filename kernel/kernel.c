#include "terminal.h"
#include "init_k.h"
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif


void kernel_main(void) 
{
	init_drivers();
	uint32_t *ptr = (uint32_t *)0xDEADBEEF;
    *ptr = 42;
	//print_welcome_message();

}