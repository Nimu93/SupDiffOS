global enablePaging
global loadPageDirectory


enablePaging:
    push ebp
    mov ebp, esp

    mov eax, cr0
    or eax, 0x80000000  ; Activer le bit de paging (PG)
    mov cr0, eax

    mov esp, ebp
    pop ebp
    ret

loadPageDirectory:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    mov cr3, eax      

    mov esp, ebp
    pop ebp
    ret
