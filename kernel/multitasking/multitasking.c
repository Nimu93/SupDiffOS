#include "multitasking.h"

extern void switch_to_task(struct thread_control_block* next_thread);
void schedule(void);
void create_kernel_task(struct thread_control_block* tcb, void (*entry_point)(), void *adress_space);

struct thread_control_block* current_task_TCB = NULL;

struct circular_linked_list *task_list;

void task_exit(void) {
    disable_interrupts();
    current_task_TCB->state = THREAD_STATE_TERMINATED;
    enable_interrupts();
    schedule(); 
    for (;;) { asm volatile("hlt"); }
}

void task_create(void (*entry_point)()) {
    struct thread_control_block* tcb = (struct thread_control_block*)kmalloc(sizeof(struct thread_control_block));
    create_kernel_task(tcb, entry_point, NULL);
}

void create_kernel_task(struct thread_control_block* tcb, void (*entry_point)(), void *adress_space) {
    // Allocate a fresh kernel stack and prime it with the values switch_to_task expects
    uint8_t *stack_base = kmalloc(KERNEL_STACK_SIZE);
    uint32_t *esp = (uint32_t *)(stack_base + KERNEL_STACK_SIZE);

    /* Layout expected by switch_to_task: [EBP][EDI][ESI][EBX][RET] */
    *(--esp) = (uint32_t)task_exit;   // Return address when entry_point returns
    *(--esp) = (uint32_t)entry_point; // First RET jumps here
    *(--esp) = 0; // EBX
    *(--esp) = 0; // ESI
    *(--esp) = 0; // EDI
    *(--esp) = 0; // EBP

    tcb->esp = esp;
    tcb->esp0 = stack_base + KERNEL_STACK_SIZE;
    tcb->state = THREAD_STATE_READY;
    tcb->time_spent = 0;
    tcb->next = NULL;
    cll_insert(task_list, tcb);
}

uint64_t last_count = 0;
uint64_t current_count = 0;
uint64_t elapsed = 0;

void update_time_used(void) {
    current_count = read_current_tick();
    elapsed = current_count - last_count;
    last_count = current_count;
    current_task_TCB->time_spent += elapsed;
}


void init_multitasking() {
    // Implementation of multitasking initialization
    printf("Initializing Multitasking...\n");
    uint32_t current_esp;
    static struct thread_control_block boot_task;
    asm volatile("mov %%esp, %0" : "=r"(current_esp)); // Get current ESP
    boot_task.esp = (void *)current_esp;
    boot_task.esp0 = NULL;
    boot_task.state = THREAD_STATE_RUNNING;
    boot_task.next = NULL;
    current_task_TCB = &boot_task;
    task_list = init_circular_linked_list();
    cll_insert(task_list, &boot_task);
    update_time_used();
    insert_tick_action((void*)schedule);
}

void schedule(void) {
    printf("Schedule");
    if (cll_is_empty(task_list)) return;

    struct thread_control_block *next = NULL;
    struct thread_control_block *first_seen = NULL;
    bool first_pass = true;

    do {
        next = cll_remove(task_list);              
        if (!first_seen) first_seen = next;

        if (next->state == THREAD_STATE_TERMINATED) {
            kfree((uint8_t *)next->esp0 - KERNEL_STACK_SIZE);
            kfree(next);                                   
            if (cll_is_empty(task_list)) return;
            continue;
        }

        cll_insert(task_list, next);
        if (next->state == THREAD_STATE_READY) {
            disable_interrupts();
            switch_to_task(next);
            enable_interrupts();
            update_time_used();
            return;
        }
        first_pass = false;
    } while (first_pass || next != first_seen);
}
