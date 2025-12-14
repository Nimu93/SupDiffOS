#ifndef PROCESS_H
#define PROCESS_H

#include <types.h>

struct process_control_block {
    uint32_t* esp;      // Pointer to the thread's stack
    uint32_t* esp0;          // Pointer 
    uint32_t process_id;         // Identifier of the process
    uint8_t state;               // Process state (e.g., running, ready, blocked)
    struct process_control_block *next;
    void (*entry_point)(); // Entry point of the process
};

struct process {
    uint32_t process_id;         // Identifier of the process
    struct process_control_block *pcb; // Pointer to the PCB

}

struct process* create_process(void (*entry_point)());;
void switch_to_process(struct process* next_process);

#endif // PROCESS_H
