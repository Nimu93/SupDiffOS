#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include "sched/process.h"

#define KERNEL_STACK_SIZE 0x4000  /* 16 KB kernel stack per task */
#define USER_STACK_SIZE   0x4000  /* 16 KB user stack per ring-3 task */

enum THREAD_STATE {
    THREAD_STATE_READY = 0,
    THREAD_STATE_RUNNING,
    THREAD_STATE_BLOCKED,
    THREAD_STATE_TERMINATED
};

struct thread_control_block {
    void *esp;       /* saved kernel ESP (context-switch state) */
    void *esp0;      /* top of kernel stack used by TSS.esp0 */
    void *virt_addr;
    struct thread_control_block *next;
    uint64_t time_spent;
    enum THREAD_STATE state;
    void *user_stack_base; /* base of user-mode stack allocation (NULL for kernel tasks) */
    uint32_t tid;
    struct process *process;
    struct thread_control_block *process_next;
};

void thread_init_boot(struct thread_control_block *thread, void *esp, struct process *process);
void thread_destroy(struct thread_control_block *thread);

void thread_create_kernel(void (*entry_point)());
void thread_create_user(void (*entry_point)());
void thread_exit(void) __attribute__((noreturn));
void thread_unreachable(void) __attribute__((noreturn));

#endif
