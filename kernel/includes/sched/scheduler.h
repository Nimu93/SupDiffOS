#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "arch/x86/irq.h"
#include "arch/x86/timer.h"
#include "lib/circular_linked_list.h"
#include "sched/thread.h"

extern struct thread_control_block *current_task_TCB;

void init_multitasking(void);
void schedule(void);
void scheduler_enqueue_thread(struct thread_control_block *thread);
void scheduler_defer_thread_destroy(struct thread_control_block *thread);

#endif
