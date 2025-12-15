#ifndef MULTITASKING_H
#define MULTITASKING_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "kheap.h"
#include "irq.h"
#include "circular_linked_list.h"
#include "timer.h"


#define KERNEL_STACK_SIZE 0x4000 // 16 KB

enum THREAD_STATE {
    THREAD_STATE_READY = 0,
    THREAD_STATE_RUNNING,
    THREAD_STATE_BLOCKED,
    THREAD_STATE_TERMINATED
};

struct thread_control_block {
    void *esp;
    void *esp0;
    void *virt_addr;
    struct thread_control_block *next;
    uint64_t time_spent;
    enum THREAD_STATE state;

};

extern struct thread_control_block *current_task_TCB;

void task_create(void (*entry_point)());
void init_multitasking();


#endif // MULTITASKING_H
