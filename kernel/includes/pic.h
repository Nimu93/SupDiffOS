#ifndef PIC_H
#define PIC_H


#include "asm_io.h" 
#include <stdint.h>

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

void pic_remap(int offset1, int offset2);

void IRQ_set_mask(uint8_t mask);

void IRQ_clear_mask(uint8_t mask);

#endif // PIC_H
