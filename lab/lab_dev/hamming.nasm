extern printf

SECTION  .rodata
hamming_fmt_str:
    db     'Hamming number: %lu', 0ah, 0


; Fastcall: no stack manipulations
min2:
    cmp rdi, rsi
    jg  min2_greater

    mov rax, rdi
    jmp min2_exit

min2_greater:
    mov rax, rsi

min2_exit:
    ret


min3:
    call min2
    mov  rdi, rax
    mov  rsi, rdx
    call min2
    ret


global hamming
hamming:
    push rbp
    mov  rbp, rsp

    ; Save argument, so '%rdi' could be reused
    mov r15, rdi

    ; Using VLA stack allocation
    imul rdi, 8
    sub  rsp, rdi

    xor r8,  r8   ; i - %r8
    xor r9,  r9   ; j - %r9
    xor r10, r10  ; k - %10

    mov r11, 2    ; ham2 - %r11
    mov r12, 3    ; ham3 - %r12
    mov r13, 5    ; ham5 - %r13

    mov QWORD [rsp], 1
    mov rcx, 1    ; n - index

hamming_for_loop:
    cmp r15, rcx
    je  hamming_for_end

    mov  rdi, r11
    mov  rsi, r12
    mov  rdx, r13
    call min3

    mov [rsp + rcx * 8], rax

    cmp rax, r11
    jne hamming_for_ham2_end

    inc r8
    mov rbx, [rsp + r8 * 8]

    imul rbx, 2
    mov  r11, rbx

hamming_for_ham2_end:
    cmp rax, r12
    jne hamming_for_ham3_end

    inc r9
    mov rbx, [rsp + r9 * 8]

    imul rbx, 3
    mov  r12, rbx

hamming_for_ham3_end:
    cmp rax, r13
    jne hamming_for_ham5_end

    inc r10
    mov rbx, [rsp + r10 * 8]

    imul rbx, 5
    mov  r13, rbx

hamming_for_ham5_end:
    inc rcx
    jmp hamming_for_loop

hamming_for_end:
    dec r15
    mov rax, [rsp + r15 * 8]

    mov rsp, rbp
    pop rbp
    ret

global main
main:
    push rbp
    mov  rbp, rsp

    mov  rdi, 62
    call hamming

    mov  rsi, rax
    lea  rdi, hamming_fmt_str
    call printf

    xor rax, rax
    ret
