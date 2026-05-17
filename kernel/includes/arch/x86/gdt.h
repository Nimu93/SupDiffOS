#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stddef.h>

/* GDT segment selectors */
#define GDT_KERNEL_CODE  0x08
#define GDT_KERNEL_DATA  0x10
#define GDT_USER_CODE    0x1B  /* 0x18 | RPL 3 */
#define GDT_USER_DATA    0x23  /* 0x20 | RPL 3 */
#define GDT_TSS_SEL      0x28

struct gdt_entry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* x86 32-bit Task State Segment — only esp0/ss0 are critical for ring-3 switching */
struct tss_entry {
    uint32_t prev_tss;
    uint32_t esp0;       /* kernel stack pointer loaded on ring-3 -> ring-0 transition */
    uint32_t ss0;        /* kernel stack segment (GDT_KERNEL_DATA) */
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint16_t trap;
    uint16_t iomap_base;
} __attribute__((packed));

void gdt_init();
void tss_init();
void tss_update_esp0(uint32_t esp0);

#endif