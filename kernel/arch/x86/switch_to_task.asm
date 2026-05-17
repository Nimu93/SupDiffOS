extern current_task_TCB
global switch_to_task

; Offsets pour struct thread_control_block (32-bit)
struc TCB
    .ESP:   resd 1    ; 0  -> void *esp
    .ESP0:  resd 1    ; 4  -> void *esp0
    .VIRT_ADDR:   resd 1    ; 8 -> void *virt_addr
    .NEXT:  resd 1    ; 12 -> struct thread_control_block *next
    .TIME_SPENT: resq 1 ; 16 -> uint64_t time_spent
    .STATE: resd 1    ; 24 -> enum THREAD_STATE state
    .USER_STACK_BASE: resd 1
    .TID: resd 1
    .PROCESS: resd 1
    .PROCESS_NEXT: resd 1
endstruc


switch_to_task:
    ;Save previous task's state

    ;Notes:
    ;  For cdecl; EAX, ECX, and EDX are already saved by the caller and don't need to be saved again
    ;  EIP is already saved on the stack by the caller's "CALL" instruction
    ;  The task isn't able to change VIRT_ADDR so it doesn't need to be saved
    ;  Segment registers are constants (while running kernel code) so they don't need to be saved

    pushfd                        ; Save EFLAGS (includes IF) so fresh tasks can restore IF=1 via initial stack
    push ebx
    push esi
    push edi
    push ebp

    mov edi,[current_task_TCB]    ; edi = address of the previous task's "thread control block"
    mov [edi+TCB.ESP],esp         ; Save ESP for previous task's kernel stack in the thread's TCB

    ;Load next task's state

    mov esi,[esp+(5+1)*4]         ; esi = address of the next task's "thread control block" (parameter passed on stack)
    mov [current_task_TCB],esi    ; Current task's TCB is the next task TCB

    mov esp,[esi+TCB.ESP]         ; Load ESP for next task's kernel stack from the thread's TCB
    ; mov eax,[esi+TCB.VIRT_ADDR]   ; eax = address of page directory for next task
    mov ebx,[esi+TCB.ESP0]        ; ebx = address for the top of the next task's kernel stack
    ; mov [TSS.ESP0],ebx           ; Adjust the ESP0 field in the TSS (used by CPU for for CPL=3 -> CPL=0 privilege level changes)
    ; mov ecx,cr3                   ; ecx = previous task's virtual address space

    ;cmp eax,ecx                   ; Does the virtual address space need to being changed?
    ;je .doneVAS                   ; no, virtual address space is the same, so don't reload it and cause TLB flushes
    ; mov cr3,eax                   ; yes, load the next task's virtual address space
.doneVAS:

    pop ebp
    pop edi
    pop esi
    pop ebx
    popfd                         ; Restore EFLAGS (re-enables interrupts for fresh tasks via 0x202 initial value)

    ret                           ;Load next task's EIP from its kernel stack
