#include "idt.h"

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;

static void (*exceptionISRFunctions[32])(registers_t *registers) = {NULL}; 

static void (*exceptionIRQFunctions[16])(registers_t *registers) = {NULL}; 

void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08;
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}

void print_all_registers_t(registers_t *registers)
{
    printf("Registers:\n");
    /*printf("ds: %x\n", registers.ds);
    printf("edi: %x\n", registers.edi);
    printf("esi: %x\n", registers.esi);
    printf("ebp: %x\n", registers.ebp);
    printf("esp: %x\n", registers.esp);
    printf("ebx: %x\n", registers.ebx);
    printf("edx: %x\n", registers.edx);
    printf("ecx: %x\n", registers.ecx);*/
    printf("eax: %x\n", registers->eax);
    printf("int_no: %d\n", registers->int_no);
    printf("err_code: %x\n", registers->err_code);
    printf("eip: %x\n", registers->eip);
    printf("cs: %x\n", registers->cs);
    printf("eflags: %x\n", registers->eflags);
    printf("useresp: %x\n", registers->useresp);
    printf("ss: %x\n", registers->ss);
}

void generic_fault(registers_t* regs) {
    printf("Generic fault! Error: 0x%x\n", regs->err_code);
   // print_all_registers_t(regs);
}

uint32_t get_current_cr3() {
    uint32_t cr3;
    asm volatile ("mov %%cr3, %0" : "=r"(cr3));
    return cr3;
}

void exception_handler(registers_t* registers) {
    if (registers->int_no < 32)
    {
        exceptionISRFunctions[registers->int_no](registers);
    }
    else
        exceptionIRQFunctions[registers->int_no - 32](registers);
}

void exception_handler_no_err(registers_t* registers) {
    print_all_registers_t(registers);
    printf("Exception occurred without error code\n");
    return;
}
static bool vectors[256];

extern void* isr_stub_table[];

extern void *irq_stub_table[];

void isr_init(void)
{
    for (int i = 0; i < 32; i++) {
        exceptionISRFunctions[i] = generic_fault;
    }
    exceptionISRFunctions[14] = page_fault_handler;
    exceptionISRFunctions[21] = control_protection_handler;
    exceptionISRFunctions[20] = control_protection_handler;
    exceptionISRFunctions[13] = gpf_handler;
    exceptionISRFunctions[8] = double_fault_handler;
    exceptionISRFunctions[6] = invalid_opcode_handler;
}

void irq_init(void)
{
    for (int i = 0; i < 16; i++) {
        exceptionIRQFunctions[i] = generic_fault;
    }
    exceptionIRQFunctions[0] = timer_handler;
    exceptionIRQFunctions[1] = keyboard_handler;
}

void idt_init() {
    pic_remap(0x20, 0x28); // Remap the PICs
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;
    isr_init();
    irq_init();
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = 1;
    }
    for (uint8_t vector = 32; vector <= 47; vector++) {
        idt_set_descriptor(vector, irq_stub_table[vector - 32], 0x8E);
        vectors[vector] = 1;
    }
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}
