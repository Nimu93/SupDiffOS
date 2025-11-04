[BITS 32]

; ============================================================
;  INTERRUPTS.ASM
;  Defines all ISRs (0–31) and IRQs (32–47)
;  Reuses a shared isr_common_stub that calls into C.
; ============================================================

; ------------------------------------------------------------
; External C handler (your main interrupt dispatcher in C)
; ------------------------------------------------------------
extern exception_handler       ; void exception_handler(registers_t *r)

; ------------------------------------------------------------
; Shared stub for ISRs *and* IRQs
; Saves all registers, calls C, restores, returns with iret
; ------------------------------------------------------------
global common_stub
common_stub:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10           ; kernel data segment selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp           ; pointer to registers_t
    push eax               ; push argument for C function
    call exception_handler
    add esp, 4             ; clean up stack (argument)

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, byte 8        ; remove error code + interrupt number
    iret


; ============================================================
; Macros for ISR and IRQ stubs
; ============================================================

; --- ISRs with error code automatically pushed by CPU ---
%macro isr_err_stub 1
global isr_stub_%1
isr_stub_%1:
    push byte %1           ; push interrupt number
    jmp common_stub
%endmacro

; --- ISRs without error code ---
%macro isr_no_err_stub 1
global isr_stub_%1
isr_stub_%1:
    push byte 0            ; dummy error code
    push byte %1           ; interrupt number
    jmp common_stub
%endmacro

; --- IRQ stubs (for hardware interrupts 0–15) ---
%macro irq_stub 1
global irq_stub_%1
irq_stub_%1:
    push byte 0            ; dummy error code
    push byte (32 + %1)    ; interrupt number (after PIC remap)
    jmp common_stub
%endmacro


; ============================================================
; ISRs 0–31
; ============================================================

isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

; Table of ISR stub addresses
global isr_stub_table
isr_stub_table:
%assign i 0
%rep 32
    dd isr_stub_%+i
%assign i i+1
%endrep


; ============================================================
; IRQs 0–15  (after PIC remapping to 0x20–0x2F)
; ============================================================

irq_stub 0
irq_stub 1
irq_stub 2
irq_stub 3
irq_stub 4
irq_stub 5
irq_stub 6
irq_stub 7
irq_stub 8
irq_stub 9
irq_stub 10
irq_stub 11
irq_stub 12
irq_stub 13
irq_stub 14
irq_stub 15

; Table of IRQ stub addresses
global irq_stub_table
irq_stub_table:
%assign i 0
%rep 16
    dd irq_stub_%+i
%assign i i+1
%endrep
