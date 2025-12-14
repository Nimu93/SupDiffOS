#include "memory.h"

static uint32_t page_directory[1024] __attribute__((aligned(4096)));

static uint32_t page_table[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_paging(void) {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 0x3; // PRESENT + RW
    }
    page_directory[0] = (uint32_t)first_page_table | 0x3; // PRESENT + RW
    // Initialize remaining PDEs as not present
    for (int i = 1; i < 1024; i++) {
        page_directory[i] = 0x0;
    }
    loadPageDirectory(page_directory);
    enablePaging();
}

extern uint32_t endkernel; // Defined in linker.ld

uint32_t alloc_physpage() {
    static uint32_t next_free_page = (uint32_t)&endkernel;
    // Align to 4KB boundary
    next_free_page = (next_free_page + 0xFFF) & ~0xFFF;
    printf("Allocating page at 0x%x\n", next_free_page);
    uint32_t allocated_page = next_free_page;
    next_free_page += 0x1000; // 4KB
    return allocated_page;
}

void map_page(void *physaddr, void *virtualaddr, unsigned int flags) {
    uint32_t vaddr = (uint32_t)virtualaddr;
    uint32_t pdindex = vaddr >> 22;
    uint32_t ptindex = (vaddr >> 12) & 0x03FF;

    // Validate pdindex
    if (pdindex >= 1024) {
        printf("Invalid PDE index!\n");
        return;
    }

    // Allocate a new page table if the PDE is not present
    if (!(page_directory[pdindex] & 0x1)) {
        uint32_t new_pt_phys = alloc_physpage();      // physical page for the page table itself
        memset((void*)new_pt_phys, 0, PAGE_SIZE);     // zero the new page table (identity mapped)
        page_directory[pdindex] = new_pt_phys | flags | 0x1;
    }

    // Get the page table
    uint32_t *pt = (uint32_t*)(page_directory[pdindex] & ~0xFFF);
    pt[ptindex] = (uint32_t)physaddr | flags | 0x1;

    // Invalidate TLB entry
    asm volatile("invlpg (%0)" ::"r"(virtualaddr) : "memory");
}

void *allocate_page(void* fault_addr) {
    //align fault_addr to 4KB
    fault_addr = (void*)((uint32_t)fault_addr & 0xFFFFF000);
    uint32_t phys_addr = alloc_physpage();
    map_page((void*)phys_addr, fault_addr, PAGE_PRESENT | PAGE_RW);
    return fault_addr;
}
