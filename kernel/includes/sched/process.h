#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

struct thread_control_block;

struct process {
    uint32_t pid;
    void *address_space;
    struct thread_control_block *main_thread;
    struct thread_control_block *threads;
};

void process_init_kernel(void);
struct process *process_kernel(void);
struct process *process_create(void);
void process_attach_thread(struct process *process, struct thread_control_block *thread);
void process_detach_thread(struct thread_control_block *thread);

#endif
