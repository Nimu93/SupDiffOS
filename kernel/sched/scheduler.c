#include "sched/scheduler.h"
#include "arch/x86/gdt.h"

extern void switch_to_task(struct thread_control_block *next_thread);

struct thread_control_block *current_task_TCB = NULL;

static struct circular_linked_list *runqueue;
/* Threads in this list have already been removed from the runqueue. */
static struct thread_control_block *terminated_threads = NULL;
static uint64_t last_count = 0;

void scheduler_enqueue_thread(struct thread_control_block *thread) {
    cll_insert(runqueue, thread);
}

void scheduler_defer_thread_destroy(struct thread_control_block *thread) {
    /*
     * A thread cannot free its own kernel stack while it is still executing on
     * it. Keep it here until another thread reaches the scheduler.
     */
    thread->next = terminated_threads;
    terminated_threads = thread;
}

static void cleanup_terminated_threads(void) {
    struct thread_control_block **link = &terminated_threads;

    while (*link) {
        struct thread_control_block *thread = *link;
        if (thread == current_task_TCB) {
            /* Still running on this stack; destruction must wait. */
            link = &thread->next;
            continue;
        }

        *link = thread->next;
        thread_destroy(thread);
    }
}

static void update_time_used(void) {
    uint64_t current_count = read_current_tick();

    current_task_TCB->time_spent += current_count - last_count;
    last_count = current_count;
}

void init_multitasking(void) {
    printf("Initializing Multitasking...\n");

    uint32_t current_esp;
    static struct thread_control_block boot_task;

    asm volatile("mov %%esp, %0" : "=r"(current_esp));

    runqueue = init_circular_linked_list();
    process_init_kernel();
    thread_init_boot(&boot_task, (void *)current_esp, process_kernel());

    /* The boot context becomes the first schedulable kernel thread. */
    current_task_TCB = &boot_task;
    scheduler_enqueue_thread(&boot_task);
    update_time_used();
    insert_tick_action((void *)schedule);
}

void schedule(void) {
    if (cll_is_empty(runqueue)) {
        return;
    }

    cleanup_terminated_threads();

    /*
     * Round-robin policy: pop the current head, inspect it, then append it back
     * if it is still runnable. Stop once we loop back to the first thread seen.
     */
    struct thread_control_block *next = NULL;
    struct thread_control_block *first_seen = NULL;
    bool first_pass = true;

    do {
        next = cll_remove(runqueue);
        if (!first_seen) {
            first_seen = next;
        }

        if (next->state == THREAD_STATE_TERMINATED) {
            if (next == current_task_TCB) {
                scheduler_defer_thread_destroy(next);
            } else {
                thread_destroy(next);
            }
            if (cll_is_empty(runqueue)) {
                return;
            }
            continue;
        }

        scheduler_enqueue_thread(next);
        if (next->state == THREAD_STATE_READY && next != current_task_TCB) {
            if (current_task_TCB->state == THREAD_STATE_RUNNING) {
                current_task_TCB->state = THREAD_STATE_READY;
            }
            next->state = THREAD_STATE_RUNNING;
            /* TSS.esp0 must match the kernel stack of the next ring-3 thread. */
            tss_update_esp0((uint32_t)next->esp0);
            disable_interrupts();
            switch_to_task(next);
            enable_interrupts();
            return;
        }
        first_pass = false;
    } while (first_pass || next != first_seen);
}
