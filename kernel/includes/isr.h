#ifndef ISR_H
#define ISR_H
//File used for Interrupt Service Routines

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "memory.h"

typedef struct {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
 } registers_t;

	

static void (*exceptionFunctions[32])(registers_t *registers) = {NULL}; 

void exception_handler(registers_t* registers);

void exception_handler_no_err(registers_t* registers);

void isr_init(void);

#endif
