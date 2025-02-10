#include "memory.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_paging(void) {
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | 3; 
    }

    for (int i = 0; i < 1024; i++) {
        page_directory[i] = 0x00000002; 
    }
    printf("page_directory = %x\n", page_directory);
    page_directory[0] = ((uint32_t) first_page_table) | 3; 

    loadPageDirectory(page_directory);

    enablePaging();
}
