#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define PAGE_PRESENT  0x1
#define PAGE_RW       0x2
#define PAGE_USER     0x4
#define PAGE_SIZE     0x1000

void init_paging(void);
extern void loadPageDirectory(long unsigned int*);
extern void enablePaging();
void *allocate_page(void* fault_addr);

extern uint32_t endkernel;

#endif