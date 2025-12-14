#include "multitasking.h"

extern void switch_to_task(struct thread_control_block* next_thread);
struct thread_control_block* current_task_TCB = NULL;

void create_kernel_task(struct thread_control_block* tcb, void (*entry_point)(), void *adress_space) {
    // Allocate a fresh kernel stack and prime it with the values switch_to_task expects
    uint8_t *stack_base = kmalloc(KERNEL_STACK_SIZE);
    uint32_t *esp = (uint32_t *)(stack_base + KERNEL_STACK_SIZE);

    *(--esp) = (uint32_t)entry_point; // Return EIP for the initial ret
    *(--esp) = 0; // EBX
    *(--esp) = 0; // ESI
    *(--esp) = 0; // EDI
    *(--esp) = 0; // EBP

    tcb->esp = esp;
    tcb->esp0 = stack_base + KERNEL_STACK_SIZE;
    tcb->state = THREAD_STATE_READY;
    //tcb->virt_addr = adress_space;
    tcb->next = NULL;

    

}

struct thread_control_block *second_task;
struct thread_control_block *first_task;

void first_task_entry() {
    while (true)
    {
        printf("First Task Running\n");
        disable_interrupts();
        switch_to_task(second_task);
        enable_interrupts();
    }
    
}

void second_task_entry() {
    while (true)
    {
        printf("Second Task Running\n");
        disable_interrupts();
        switch_to_task(first_task);
        enable_interrupts();
    }
    
}

void mini_test() {
    first_task = kmalloc(sizeof(struct thread_control_block));
    second_task = kmalloc(sizeof(struct thread_control_block));
    create_kernel_task(first_task, first_task_entry, NULL);
    create_kernel_task(second_task, second_task_entry, NULL);
    switch_to_task(first_task);
    //current_task_TCB = first_task;

}

static struct circular_linked_list *tasks;

void init_multitasking() {
    // Implementation of multitasking initialization
    printf("Initializing Multitasking...\n");
    uint32_t current_esp;
    static struct thread_control_block boot_task;
    asm volatile("mov %%esp, %0" : "=r"(current_esp));
    boot_task.esp = (void *)current_esp;
    boot_task.esp0 = NULL;
    boot_task.state = THREAD_STATE_RUNNING;
    boot_task.next = NULL;
    current_task_TCB = &boot_task;
    //mini_test();
}


void schedule() {
    // Implementation of the scheduler
}