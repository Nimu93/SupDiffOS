#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void init_paging(void);
extern void loadPageDirectory(long unsigned int*);
extern void enablePaging();

#endif