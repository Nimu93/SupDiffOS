#include "arch/x86/gdt.h"

#define GDT_ENTRIES 6

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gp;
struct tss_entry tss;  /* zero-initialised by BSS; only esp0/ss0/iomap_base need setting */

static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access      = access;
}

void gdt_init() {
    gp.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gp.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0,       0x00, 0x00);  /* null */
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF);  /* kernel code  ring 0 */
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF);  /* kernel data  ring 0 */
    gdt_set_gate(3, 0, 0xFFFFF, 0xFA, 0xCF);  /* user code    ring 3 */
    gdt_set_gate(4, 0, 0xFFFFF, 0xF2, 0xCF);  /* user data    ring 3 */
    /* entry 5 (TSS) filled by tss_init() */

    asm volatile("lgdt %0" : : "m"(gp));

    asm volatile(
        "ljmp $0x08, $flush_cs\n"
        "flush_cs:\n"
        "movw $0x10, %ax\n"
        "movw %ax, %ds\n"
        "movw %ax, %es\n"
        "movw %ax, %fs\n"
        "movw %ax, %gs\n"
        "movw %ax, %ss\n"
    );
}

void tss_init() {
    uint32_t base  = (uint32_t)&tss;
    uint32_t limit = sizeof(struct tss_entry) - 1;

    /* TSS descriptor: present, DPL=0, type=0x9 (32-bit available TSS), byte granularity */
    gdt_set_gate(5, base, limit, 0x89, 0x00);

    tss.ss0        = GDT_KERNEL_DATA;
    tss.esp0       = 0;  /* updated by tss_update_esp0() before every ring-3 switch */
    tss.iomap_base = sizeof(struct tss_entry);  /* no I/O permission bitmap */

    asm volatile("ltr %%ax" :: "a"((uint16_t)GDT_TSS_SEL));
}

void tss_update_esp0(uint32_t esp0) {
    tss.esp0 = esp0;
}