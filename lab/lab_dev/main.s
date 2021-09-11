.section .data
string:
    .asciz "Hello, world!\n"
    .equ len_string, . - string - 1
pstring:
    .long 1, 2, 3, 4, 5, 6
    .byte 'H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '\n', '\0'

.section .bss
    .equ len_string2, 1024
    .comm string2, len_string2

.section .text
.globl main
.type main, @function
main:
    # Initializing function's stack frame
    pushl %ebp
    movl %esp, %ebp

    .equ a, -4
    .equ b, -8
    subl $0x8, %esp

    movl $0x5, b(%ebp)

   #movl $0x80490a9, 0x80490b8
    movl $string   , pstring
    movl $string   , %eax # 08 04 90 A9

    # "Hello, world!\n"
    movb $'?', string + 12
    movb $'o', 3(%eax) # offt(%base, %index, scale) # *((base + (index * scale)) + offt)

    # Syscall
    movl $0x4, %eax
    movl $0x1, %ebx
    movl $string, %ecx
    movl $len_string, %edx
    int $0x80

    movl $0x2, %eax
    addl $0x2, %eax

if:
    cmpl $0x5, %eax
    jl if_else  # < (signed)    ; SF == 1
    jb if_else  # < (unsigned)  ; CF == 1

    je if_else  # ==            ; ZF == 1
    jz if_else  # ==            ; ZF == 1

    jne if_else # !=            ; ZF == 0
    jnz if_else # !=            ; ZF == 0

    jg if_else  # > (signed)    ; SF == 0 && ZF == 0
    ja if_else  # > (unsigned)  ; CF == 0 && ZF == 0

    jle if_else # <= (signed)   ; SF == 1 || ZF == 1
    jbe if_else # <= (unsigned) ; CF == 1 || ZF == 1

    jge if_else # >= (signed)   ; SF == 0 || ZF == 1
    jae if_else # >= (unsigned) ; CF == 0 || ZF == 1
if_then:

    jmp if_end

if_else:


if_end:

while:
    cmpl ..., ...
    je while_end

    ....

    jmp while
while_end:


    # Returning value
    xorl %eax, %eax

    # Destroying function's stack frame
    movl %ebp, %esp
    popl %ebp
    retl
