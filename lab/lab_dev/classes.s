	.file	"classes.c"
	.text
	.p2align 4,,15
	.globl	n_sum
	.type	n_sum, @function
n_sum:
.LFB0:
	.cfi_startproc
	movabsq	$206158430216, %rax
	movq	%rax, -40(%rsp)
#APP
# 17 "classes.c" 1
	leaq 16(%rbp), %rax
	movq %rax, -32(%rsp)
	leaq -176(%rbp), %rax
	movq %rax, -24(%rsp)
	
# 0 "" 2
#NO_APP
	leal	-1(%rdi), %edx
	testl	%edi, %edi
	je	.L6
	movl	-40(%rsp), %esi
	movq	-32(%rsp), %rdi
	xorl	%eax, %eax
	movq	-24(%rsp), %r8
	jmp	.L5
	.p2align 4,,10
	.p2align 3
.L10:
	movl	%esi, %ecx
	subl	$1, %edx
	addl	$8, %esi
	addq	%r8, %rcx
	addl	(%rcx), %eax
	cmpl	$-1, %edx
	je	.L9
.L5:
	cmpl	$47, %esi
	jbe	.L10
	movq	%rdi, %rcx
	subl	$1, %edx
	addq	$8, %rdi
	addl	(%rcx), %eax
	cmpl	$-1, %edx
	jne	.L5
.L9:
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE0:
	.size	n_sum, .-n_sum
	.p2align 4,,15
	.globl	n_sum_proper
	.type	n_sum_proper, @function
n_sum_proper:
.LFB12:
	.cfi_startproc
	leaq	8(%rsp), %rax
	movq	%rdx, -32(%rsp)
	leal	-1(%rdi), %edx
	movq	%rax, -64(%rsp)
	leaq	-48(%rsp), %rax
	movq	%rsi, -40(%rsp)
	movq	%rcx, -24(%rsp)
	movq	%r8, -16(%rsp)
	movq	%r9, -8(%rsp)
	movl	$8, -72(%rsp)
	movq	%rax, -56(%rsp)
	testl	%edi, %edi
	je	.L16
	movq	%rax, %r8
	movl	$8, %esi
	leaq	8(%rsp), %rdi
	xorl	%eax, %eax
	jmp	.L15
	.p2align 4,,10
	.p2align 3
.L19:
	movl	%esi, %ecx
	subl	$1, %edx
	addl	$8, %esi
	addq	%r8, %rcx
	addl	(%rcx), %eax
	cmpl	$-1, %edx
	je	.L18
.L15:
	cmpl	$47, %esi
	jbe	.L19
	movq	%rdi, %rcx
	subl	$1, %edx
	addq	$8, %rdi
	addl	(%rcx), %eax
	cmpl	$-1, %edx
	jne	.L15
.L18:
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	xorl	%eax, %eax
	ret
	.cfi_endproc
.LFE12:
	.size	n_sum_proper, .-n_sum_proper
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Sum: %d\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	movabsq	$206158430216, %rax
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movl	%eax, %ecx
	movq	%rax, (%rsp)
#APP
# 17 "classes.c" 1
	leaq 16(%rbp), %rax
	movq %rax, 8(%rsp)
	leaq -176(%rbp), %rax
	movq %rax, 16(%rsp)
	
# 0 "" 2
#NO_APP
	movl	$10, %eax
	movq	8(%rsp), %rdi
	xorl	%r8d, %r8d
	xorl	%esi, %esi
	movq	16(%rsp), %r9
	jmp	.L23
	.p2align 4,,10
	.p2align 3
.L31:
	movl	%ecx, %edx
	movl	$1, %r8d
	addl	$8, %ecx
	addq	%r9, %rdx
	addl	(%rdx), %esi
	subl	$1, %eax
	je	.L30
.L23:
	cmpl	$47, %ecx
	jbe	.L31
	movq	%rdi, %rdx
	addq	$8, %rdi
	addl	(%rdx), %esi
	subl	$1, %eax
	jne	.L23
.L30:
	testb	%r8b, %r8b
	je	.L24
	movl	%ecx, (%rsp)
.L24:
	movl	$.LC0, %edi
	xorl	%eax, %eax
	call	printf
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-4)"
	.section	.note.GNU-stack,"",@progbits
