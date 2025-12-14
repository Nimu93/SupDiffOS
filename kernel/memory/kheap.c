#include "kheap.h"

#define KHEAP_START  0xC1000000
#define KHEAP_END    0xC2000000

typedef struct block_header {
    uint32_t size;
    struct block_header* next;
} block_header_t;

static block_header_t* free_list = NULL;
static uint32_t heap_curr = KHEAP_START;
static uint32_t heap_end = KHEAP_START;

static void kheap_expand(uint32_t new_end)
{
    while (heap_end < new_end && heap_end < KHEAP_END) {
        uint32_t phys = alloc_physpage();
        map_page((void*)phys, (void*)heap_end, PAGE_PRESENT | PAGE_RW);
        heap_end += 0x1000;
    }
}
void* kmalloc(uint32_t size)
{
    if (size == 0) {
        return NULL;
    }

    // Align size to 4 bytes
    size = (size + 3) & ~3;

    block_header_t* prev = NULL;
    block_header_t* curr = free_list;

    // First-fit search
    while (curr) {
        if (curr->size >= size) {
            if (curr->size >= size + sizeof(block_header_t) + 4) {
                // Split block
                block_header_t* new_block = (block_header_t*)((uint8_t*)curr + sizeof(block_header_t) + size);
                new_block->size = curr->size - size - sizeof(block_header_t);
                new_block->next = curr->next;
                curr->size = size;
                if (prev) {
                    prev->next = new_block;
                } else {
                    free_list = new_block;
                }
            } else {
                // Use entire block
                if (prev) {
                    prev->next = curr->next;
                } else {
                    free_list = curr->next;
                }
            }
            return (uint8_t*)curr + sizeof(block_header_t);
        }
        prev = curr;
        curr = curr->next;
    }

    // No suitable block found, expand heap
    uint32_t total_size = size + sizeof(block_header_t);
    if (heap_curr + total_size > heap_end) {
        kheap_expand(heap_curr + total_size);
    }

    block_header_t* block = (block_header_t*)heap_curr;
    block->size = size;
    heap_curr += total_size;

    return (uint8_t*)block + sizeof(block_header_t);
}

void kfree(void* ptr)
{
    if (!ptr) {
        return;
    }

    block_header_t* block = (block_header_t*)((uint8_t*)ptr - sizeof(block_header_t));
    block->next = free_list;
    free_list = block;
}


