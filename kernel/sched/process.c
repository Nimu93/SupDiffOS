#include "sched/process.h"
#include "sched/thread.h"
#include "mm/kheap.h"

static struct process kernel_process;
static uint32_t next_pid = 1;

/* Process resources shared by all of its threads. */
static void process_init(struct process *process) {
    process->pid = next_pid++;
    process->address_space = NULL;
    process->main_thread = NULL;
    process->threads = NULL;
}

void process_init_kernel(void) {
    /* Kernel threads share this synthetic process. */
    process_init(&kernel_process);
}

struct process *process_kernel(void) {
    return &kernel_process;
}

struct process *process_create(void) {
    struct process *process = kmalloc(sizeof(struct process));
    process_init(process);
    return process;
}

void process_attach_thread(struct process *process, struct thread_control_block *thread) {
    if (!process) {
        return;
    }

    /* Intrusive per-process thread list; scheduler uses thread->next separately. */
    thread->process = process;
    thread->process_next = process->threads;
    process->threads = thread;
    if (!process->main_thread) {
        process->main_thread = thread;
    }
}

void process_detach_thread(struct thread_control_block *thread) {
    if (!thread->process) {
        return;
    }

    struct process *process = thread->process;
    struct thread_control_block **link = &process->threads;

    while (*link) {
        if (*link == thread) {
            *link = thread->process_next;
            break;
        }
        link = &(*link)->process_next;
    }

    if (process->main_thread == thread) {
        process->main_thread = process->threads;
    }

    thread->process = NULL;
    thread->process_next = NULL;
}
