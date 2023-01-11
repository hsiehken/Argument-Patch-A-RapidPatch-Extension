global patch_trigger

extern patch_dispatcher
extern add4_argument_patch

[bits 64]
patch_trigger:
    ; Save contexts
    push rdi
    push rax
    push rbx
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    push rsi
    push rdx
    push rcx

    mov rdi, rsp

    call patch_dispatcher ; return spatch function to go in rax

    cmp eax, 0
    je no_patch

    ;
    call rax

; there is no patch applicable
no_patch:
    ; Restore contexts
    pop rcx
    pop rdx
    pop rsi
    
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rbp
    pop rbx
    pop rax
    pop rdi

	iretq