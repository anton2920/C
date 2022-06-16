.section .rodata
hamming_fmt_str:
    .asciz "Hamming number: %lu\n"


# Fastcall: no stack manipulations
.type min2, @function
min2:
    cmpq %rsi, %rdi
    jg min2_greater

    movq %rdi, %rax
    jmp min2_exit

min2_greater:
    movq %rsi, %rax

min2_exit:
    retq


.type min3, @function
min3:
    callq min2  # Arguments are already in place
    movq %rax, %rdi
    movq %rdx, %rsi
    callq min2
    retq


.type hamming, @function
hamming:
    pushq %rbp
    movq %rsp, %rbp

    # Save argument, so '%rdi' could be reused
    movq %rdi, %r15

    # Using VLA stack allocation
    imulq $0x8, %rdi
    subq %rdi, %rsp

    xorq %r8,  %r8   # i - %r8
    xorq %r9,  %r9   # j - %r9
    xorq %r10, %r10  # k - %r10

    movq $0x2, %r11  # ham2 - %r11
    movq $0x3, %r12  # ham3 - %r12
    movq $0x5, %r13  # ham5 - %r13

    movq $0x1, (%rsp)  # first hamming number is '1'
    movq $0x1, %rcx  # n - index

hamming_for_loop:
    cmpq %rcx, %r15
    je hamming_for_end

    movq %r11, %rdi
    movq %r12, %rsi
    movq %r13, %rdx
    callq min3

    movq %rax, (%rsp, %rcx, 8)

    cmpq %r11, %rax
    jne hamming_for_ham2_end

    incq %r8
    movq (%rsp, %r8, 8), %rbx

    imulq $0x2, %rbx
    movq %rbx, %r11

hamming_for_ham2_end:
    cmpq %r12, %rax
    jne hamming_for_ham3_end

    incq %r9
    movq (%rsp, %r9, 8), %rbx

    imulq $0x3, %rbx
    movq %rbx, %r12

hamming_for_ham3_end:
    cmpq %r13, %rax
    jne hamming_for_ham5_end

    incq %r10
    movq (%rsp, %r10, 8), %rbx

    imulq $0x5, %rbx
    movq %rbx, %r13

hamming_for_ham5_end:
    incq %rcx
    jmp hamming_for_loop

hamming_for_end:
    decq %r15
    movq (%rsp, %r15, 8), %rax

    movq %rbp, %rsp
    popq %rbp
    retq


.type main, @function
.globl main
main:
    pushq %rbp
    movq %rsp, %rbp

    movq $2, %rdi
    callq hamming

    movq %rax, %rsi
    movq $hamming_fmt_str, %rdi
    callq printf

    xorq %rax, %rax
    retq
