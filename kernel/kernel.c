#include "drivers/terminal.h"
#include "init/init_k.h"
/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "mm/memory.h"
#include "sched/scheduler.h"
#include <usyscall.h>

void idle_task(void) {
	for (;;) {
		asm volatile("hlt");
	}
}

/* Ring-3 user task — uses INT 0x80 syscalls, must NOT call printf or hlt directly */
void user_task(void) {
	char input[8];
	int pid = getpid();
	(void)pid;

	for (;;) {
		print("user task: syscall write\n");
		write(1, "user task: direct write syscall\n", 32);
		syscall3(SYS_READ, 0, (int)input, sizeof(input));
		exit(0);
		/* no hlt — hlt is privileged; the PIT preempts us */
	}
}

void kernel_main(void)
{
	init_drivers();
	printf("Kernel initialized successfully!\n");
	init_multitasking();
	thread_create_user(user_task);
	thread_create_kernel(idle_task);
	for(;;);
}
