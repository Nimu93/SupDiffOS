#include "sched/thread.h"
#include "sched/scheduler.h"
#include "arch/x86/gdt.h"
#include "mm/kheap.h"
#include "mm/memory.h"

static uint32_t next_tid = 1;

#define EFLAGS_IF 0x202u

/* Initialize scheduler-owned state common to kernel and user threads. */
static void thread_init(struct thread_control_block *thread, void *esp, void *esp0, void *user_stack_base, struct process *process) {
    thread->esp = esp;
    thread->esp0 = esp0;
    thread->virt_addr = NULL;
    thread->next = NULL;
    thread->time_spent = 0;
    thread->state = THREAD_STATE_READY;
    thread->user_stack_base = user_stack_base;
    thread->tid = next_tid++;
    thread->process = NULL;
    thread->process_next = NULL;
    process_attach_thread(process, thread);
}

void thread_init_boot(struct thread_control_block *thread, void *esp, struct process *process) {
    /* The boot CPU context already exists; only wrap it in a TCB. */
    thread_init(thread, esp, NULL, NULL, process);
    thread->state = THREAD_STATE_RUNNING;
}

void thread_unreachable(void) {
    for (;;) {
        asm volatile("hlt");
    }
    __builtin_unreachable();
}

void thread_exit(void) {
    disable_interrupts();
    current_task_TCB->state = THREAD_STATE_TERMINATED;
    enable_interrupts();
    schedule();
    thread_unreachable();
}

void thread_create_kernel(void (*entry_point)()) {
    struct thread_control_block *thread = kmalloc(sizeof(struct thread_control_block));
    uint8_t *stack_base = kmalloc(KERNEL_STACK_SIZE);
    uint32_t *esp = (uint32_t *)(stack_base + KERNEL_STACK_SIZE);

    /*
     * switch_to_task restores EBP, EDI, ESI, EBX, EFLAGS, then RETs.
     * The first RET enters entry_point; if it returns, thread_exit runs.
     */
    *(--esp) = (uint32_t)thread_exit;
    *(--esp) = (uint32_t)entry_point;
    *(--esp) = EFLAGS_IF;
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;

    thread_init(thread, esp, stack_base + KERNEL_STACK_SIZE, NULL, process_kernel());
    scheduler_enqueue_thread(thread);
}

static void __attribute__((naked)) user_task_trampoline(void) {
    /* Enter ring 3 through the iret frame prepared by thread_create_user(). */
    asm volatile(
        "movw $0x23, %ax\n"
        "movw %ax, %ds\n"
        "movw %ax, %es\n"
        "movw %ax, %fs\n"
        "movw %ax, %gs\n"
        "iret"
    );
}

void thread_create_user(void (*entry_point)()) {
    struct process *process = process_create();
    struct thread_control_block *thread = kmalloc(sizeof(struct thread_control_block));
    uint8_t *kstack_base = kmalloc(KERNEL_STACK_SIZE);
    uint8_t *ustack_base = alloc_user_stack(USER_STACK_SIZE);
    uint32_t *esp = (uint32_t *)(kstack_base + KERNEL_STACK_SIZE);

    /* iret frame consumed by user_task_trampoline. */
    *(--esp) = GDT_USER_DATA;
    *(--esp) = (uint32_t)(ustack_base + USER_STACK_SIZE);
    *(--esp) = EFLAGS_IF;
    *(--esp) = GDT_USER_CODE;
    *(--esp) = (uint32_t)entry_point;

    /* Kernel-mode frame consumed first by switch_to_task. */
    *(--esp) = (uint32_t)user_task_trampoline;
    *(--esp) = EFLAGS_IF;
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;
    *(--esp) = 0;

    thread_init(thread, esp, kstack_base + KERNEL_STACK_SIZE, ustack_base, process);
    scheduler_enqueue_thread(thread);
}

void thread_destroy(struct thread_control_block *thread) {
    process_detach_thread(thread);
    /* User stacks are mapped page-by-page outside kheap; no unmap/free yet. */
    kfree((uint8_t *)thread->esp0 - KERNEL_STACK_SIZE);
    kfree(thread);
}
