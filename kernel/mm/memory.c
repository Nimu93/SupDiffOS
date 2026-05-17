#include "mm/memory.h"

#define USER_STACK_VADDR_START 0x80000000u

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_paging(void) {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    }
    page_directory[0] = (uint32_t)first_page_table | PAGE_PRESENT | PAGE_RW | PAGE_USER;
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0x0;
    }
    loadPageDirectory(page_directory);
    enablePaging();
}

extern uint32_t endkernel; // Defined in linker.ld

uint32_t alloc_physpage() {
    static uint32_t next_free_page = (uint32_t)&endkernel;

    next_free_page = (next_free_page + 0xFFF) & ~0xFFF;
    uint32_t allocated_page = next_free_page;
    next_free_page += 0x1000;
    return allocated_page;
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    uint32_t vaddr = (uint32_t)virtualaddr;
    uint32_t pdindex = vaddr >> 22;
    uint32_t ptindex = (vaddr >> 12) & 0x03FF;

    if (pdindex >= 1024) {
        printf("Invalid PDE index!\n");
        return;
    }

    if (!(page_directory[pdindex] & 0x1)) {
        uint32_t new_pt_phys = alloc_physpage();
        memset((void*)new_pt_phys, 0, PAGE_SIZE);
        page_directory[pdindex] = new_pt_phys | flags | 0x1;
    }

    uint32_t *pt = (uint32_t*)(page_directory[pdindex] & ~0xFFF);
    pt[ptindex] = (uint32_t)physaddr | flags | 0x1;

    asm volatile("invlpg (%0)" ::"r"(virtualaddr) : "memory");
}

void *allocate_page(void* fault_addr) {
    fault_addr = (void*)((uint32_t)fault_addr & 0xFFFFF000);
    uint32_t phys_addr = alloc_physpage();
    map_page((void*)phys_addr, fault_addr, PAGE_PRESENT | PAGE_RW | PAGE_USER);
    return fault_addr;
}

uint8_t *alloc_user_stack(uint32_t size) {
    static uint32_t next_user_stack = USER_STACK_VADDR_START;

    uint32_t stack_base = next_user_stack;
    uint32_t stack_end = stack_base + size;

    for (uint32_t addr = stack_base; addr < stack_end; addr += PAGE_SIZE) {
        uint32_t phys = alloc_physpage();
        map_page((void *)phys, (void *)addr, PAGE_PRESENT | PAGE_RW | PAGE_USER);
    }

    next_user_stack = stack_end;
    return (uint8_t *)stack_base;
}
