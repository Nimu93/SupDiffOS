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

void kernel_main(void) 
{
	init_drivers();
	uint32_t *ptr = (uint32_t *)0xDEADBEEF;
	//align on 4KB boundary
	    /* Test the interrupts */
		//__asm__ __volatile__("int $2");
		//__asm__ __volatile__("int $14");
	//allocate_page(ptr);
	printf("Continue after dereferencing\n");
	//*ptr = 42;
	//uint32_t *ptr2 = (uint32_t *)0xDEADDEAD;

	//*ptr2= 43;
	//printf("%d", *ptr);
	int new;
	new = 500 / 0;
	//printf("%d", new);
	//print_welcome_message();

}