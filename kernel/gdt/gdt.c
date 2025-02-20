#include "gdt.h"

struct gdt_entry gdt[3]; 
struct gdt_ptr gp;

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access = access;
}

void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (uint32_t)&gdt;

    // Null descriptor (required)
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment: base=0, limit=0xFFFFF, access=0x9A (present, ring 0, code), granularity=0xCF (4KB, 32-bit)
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);

    // Data segment: base=0, limit=0xFFFFF, access=0x92 (present, ring 0, data), granularity=0xCF
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);

    // Load GDT
    asm volatile("lgdt %0" : : "m"(gp));

    // Reload segment registers
    asm volatile("ljmp $0x08, $flush_cs\n" // Far jump to reload CS
                 "flush_cs:\n"
                 "movw $0x10, %ax\n" // Data segment (16-bit)
                 "movw %ax, %ds\n"
                 "movw %ax, %es\n"
                 "movw %ax, %fs\n"
                 "movw %ax, %gs\n"
                 "movw %ax, %ss\n");
}