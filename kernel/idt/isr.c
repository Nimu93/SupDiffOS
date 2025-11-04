#include "isr.h"

void double_fault_handler(registers_t* regs) {
    printf("Double Fault! Error: 0x%x\n", regs->err_code);
    printf("CS: 0x%x, EIP: 0x%x, ESP: 0x%x\n", regs->cs, regs->eip, regs->esp);
}

void gpf_handler(registers_t* regs) {
    static int gpf_count = 0;
    gpf_count++;
    
    printf("GPF #%d!\n", gpf_count);
    printf("Error code: 0x%x\n", regs->err_code);
    printf("At EIP: 0x%x, CS: 0x%x\n", regs->eip, regs->cs);
    printf("ESP: 0x%x, SS: 0x%x\n", regs->useresp, regs->ss);
    printf("EFLAGS: 0x%x\n", regs->eflags);

    // If we get more than one GPF, we're probably in a loop
    if (gpf_count > 1) {
        printf("Multiple GPFs detected - halting!\n");
        asm volatile("cli; hlt");
    }


}


void page_fault_handler(registers_t* registers) {
    uint32_t fault_addr;
    asm volatile ("mov %%cr2, %0" : "=r"(fault_addr));

    uint32_t error_code = registers->err_code;

    if (error_code & 0x1) {
        printf("Page fault caused by a page-protection violation\n");
        return;
    } 
    else if (error_code & 0x2) {
        fault_addr = (uint32_t)allocate_page((void*)fault_addr);
        printf("Page fault caused by a write operation and non-present page, allocation, fault address:0x%x\n", fault_addr);

        if (fault_addr % 0x1000 != 0) {
            printf("[ERROR] Fault address is not aligned to a page!\n");
        }

        uint32_t cr3 = get_current_cr3();
        asm volatile ("mov %0, %%cr3" :: "r"(cr3) : "memory");
        return; 
    }
    else {
        printf("Page fault caused by a read operation\n");
    }
   return;
}

void invalid_opcode_handler(registers_t* registers) {
    printf("Invalid opcode exception\n");
    uint8_t *faulting_instr = (uint8_t*) registers->eip;
        //TODO: Check if the instruction is a UD2 instruction
        if (faulting_instr[0] == 0x0F && faulting_instr[1] == 0x0B) {
            printf("UD2 instruction detected, skipping it!\n");
            return;
        }
}

void control_protection_handler(registers_t* regs) {
    printf("Control Protection Exception (int 0x15) — CET violation\n");
    printf("EIP=0x%x, CS=0x%x, EFLAGS=0x%x\n", regs->eip, regs->cs, regs->eflags);
    asm volatile ("cli; hlt");
}
