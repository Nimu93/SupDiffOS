#ifndef KHEAP_H
#define KHEAP_H

#include <stddef.h>
#include <stdint.h>

#include "mm/memory.h"

void *kmalloc(uint32_t size);
void kfree(void *ptr);

#endif 
