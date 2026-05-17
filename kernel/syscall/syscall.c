#include "syscall/syscall.h"
#include "arch/x86/idt.h"
#include <stdio.h>
#include "sched/scheduler.h"

extern void syscall_stub(void);

void syscall_init(void) {
    idt_set_descriptor(0x80, (void *)syscall_stub, 0xEE);
}

static int sys_exit(int status);
static int sys_write(int fd, const char *buf, uint32_t count);
static int sys_read(int fd, char *buf, uint32_t count);
static int sys_getpid(void);

void syscall_handler(registers_t *regs) {
    if (regs->eax >= NUM_SYSCALLS) {
        regs->eax = (uint32_t)-1;
        return;
    }

    int ret;

    switch (regs->eax) {
        case SYS_EXIT:
            ret = sys_exit((int)regs->ebx);
            break;
        case SYS_WRITE:
            ret = sys_write((int)regs->ebx, (const char *)regs->ecx, regs->edx);
            break;
        case SYS_READ:
            ret = sys_read((int)regs->ebx, (char *)regs->ecx, regs->edx);
            break;
        case SYS_GETPID:
            ret = sys_getpid();
            break;
        default:
            ret = -1;
            break;
    }

    regs->eax = (uint32_t)ret;
}

static int sys_exit(int status) {
    printf("[syscall] Task exit with status %d\n", status);

    thread_exit();
}

static int sys_write(int fd, const char *buf, uint32_t count) {
    if (fd != 1 && fd != 2) {
        return -1;
    }

    for (uint32_t i = 0; i < count; i++) {
        char c = buf[i];
        if (c == '\0') break;
        printf("%c", c);
    }

    return (int)count;
}

static int sys_read(int fd, char *buf, uint32_t count) {
    (void)fd;
    (void)buf;
    (void)count;
    return -1;
}

static int sys_getpid(void) {
    if (!current_task_TCB || !current_task_TCB->process) {
        return -1;
    }
    return (int)current_task_TCB->process->pid;
}
