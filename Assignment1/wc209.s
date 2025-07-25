	.file	"wc209.c"
	.globl	dfastate
	.bss
	.align 4
	.type	dfastate, @object
	.size	dfastate, 4
dfastate:
	.zero	4
	.globl	lstate
	.align 4
	.type	lstate, @object
	.size	lstate, 4
lstate:
	.zero	4
	.globl	nlines
	.align 4
	.type	nlines, @object
	.size	nlines, 4
nlines:
	.zero	4
	.globl	nwords
	.align 4
	.type	nwords, @object
	.size	nwords, 4
nwords:
	.zero	4
	.globl	nchars
	.align 4
	.type	nchars, @object
	.size	nchars, 4
nchars:
	.zero	4
	.comm	c,4,4
	.globl	n_comment
	.align 4
	.type	n_comment, @object
	.size	n_comment, 4
n_comment:
	.zero	4
	.section	.rodata
	.align 8
.LC0:
	.string	"Error: line %d: unterminated comment\n"
.LC1:
	.string	"%d %d %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	jmp	.L2
.L9:
	movl	dfastate(%rip), %eax
	testl	%eax, %eax
	jne	.L3
	movl	$1, nlines(%rip)
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	start
	movl	%eax, dfastate(%rip)
	movl	$0, lstate(%rip)
	jmp	.L2
.L3:
	movl	dfastate(%rip), %eax
	cmpl	$1, %eax
	jne	.L4
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	outside
	movl	%eax, dfastate(%rip)
	movl	$1, lstate(%rip)
	jmp	.L2
.L4:
	movl	dfastate(%rip), %eax
	cmpl	$2, %eax
	jne	.L5
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	inside
	movl	%eax, dfastate(%rip)
	movl	$2, lstate(%rip)
	jmp	.L2
.L5:
	movl	dfastate(%rip), %eax
	cmpl	$3, %eax
	jne	.L6
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	enter
	movl	%eax, dfastate(%rip)
	movl	$3, lstate(%rip)
	jmp	.L2
.L6:
	movl	dfastate(%rip), %eax
	cmpl	$4, %eax
	jne	.L7
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	comment
	movl	%eax, dfastate(%rip)
	movl	$4, lstate(%rip)
	jmp	.L2
.L7:
	movl	dfastate(%rip), %eax
	cmpl	$5, %eax
	jne	.L8
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	leave
	movl	%eax, dfastate(%rip)
	movl	$5, lstate(%rip)
	jmp	.L2
.L8:
	movl	c(%rip), %eax
	movl	%eax, %edi
	call	left
	movl	%eax, dfastate(%rip)
	movl	$6, lstate(%rip)
.L2:
	call	getchar
	movl	%eax, c(%rip)
	movl	c(%rip), %eax
	cmpl	$-1, %eax
	jne	.L9
	movl	dfastate(%rip), %eax
	cmpl	$3, %eax
	jne	.L10
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	lstate(%rip), %eax
	testl	%eax, %eax
	je	.L11
	movl	lstate(%rip), %eax
	cmpl	$1, %eax
	je	.L11
	movl	lstate(%rip), %eax
	cmpl	$6, %eax
	jne	.L10
.L11:
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
.L10:
	movl	c(%rip), %eax
	cmpl	$-1, %eax
	jne	.L12
	movl	dfastate(%rip), %eax
	cmpl	$4, %eax
	je	.L13
	movl	dfastate(%rip), %eax
	cmpl	$5, %eax
	jne	.L12
.L13:
	movl	n_comment(%rip), %edx
	movq	stderr(%rip), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movl	$1, %eax
	jmp	.L14
.L12:
	movl	nchars(%rip), %ecx
	movl	nwords(%rip), %edx
	movl	nlines(%rip), %eax
	movl	%eax, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
.L14:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.globl	start
	.type	start, @function
start:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L16
	movl	$1, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L18
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	jmp	.L18
.L16:
	cmpl	$47, -4(%rbp)
	jne	.L19
	movl	$3, dfastate(%rip)
	jmp	.L18
.L19:
	movl	$2, dfastate(%rip)
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L18:
	movl	dfastate(%rip), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	start, .-start
	.globl	outside
	.type	outside, @function
outside:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L22
	movl	$1, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L24
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	jmp	.L24
.L22:
	cmpl	$47, -4(%rbp)
	jne	.L25
	movl	$3, dfastate(%rip)
	jmp	.L24
.L25:
	movl	$2, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
.L24:
	movl	dfastate(%rip), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	outside, .-outside
	.globl	inside
	.type	inside, @function
inside:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L28
	movl	$1, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L30
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	jmp	.L30
.L28:
	cmpl	$47, -4(%rbp)
	jne	.L31
	movl	$3, dfastate(%rip)
	jmp	.L30
.L31:
	movl	$2, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L30:
	movl	dfastate(%rip), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	inside, .-inside
	.globl	enter
	.type	enter, @function
enter:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L34
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L35
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
.L35:
	movl	lstate(%rip), %eax
	testl	%eax, %eax
	je	.L36
	movl	lstate(%rip), %eax
	cmpl	$1, %eax
	je	.L36
	movl	lstate(%rip), %eax
	cmpl	$6, %eax
	jne	.L37
.L36:
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	jmp	.L38
.L37:
	movl	lstate(%rip), %eax
	cmpl	$2, %eax
	je	.L39
	movl	lstate(%rip), %eax
	cmpl	$3, %eax
	jne	.L38
.L39:
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L38:
	movl	$1, dfastate(%rip)
	jmp	.L40
.L34:
	cmpl	$42, -4(%rbp)
	jne	.L41
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	$4, dfastate(%rip)
	movl	nlines(%rip), %eax
	movl	%eax, n_comment(%rip)
	jmp	.L40
.L41:
	cmpl	$47, -4(%rbp)
	jne	.L42
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	lstate(%rip), %eax
	testl	%eax, %eax
	je	.L43
	movl	lstate(%rip), %eax
	cmpl	$1, %eax
	je	.L43
	movl	lstate(%rip), %eax
	cmpl	$6, %eax
	jne	.L44
.L43:
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
.L44:
	movl	$3, dfastate(%rip)
	jmp	.L40
.L42:
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	lstate(%rip), %eax
	testl	%eax, %eax
	je	.L45
	movl	lstate(%rip), %eax
	cmpl	$1, %eax
	je	.L45
	movl	lstate(%rip), %eax
	cmpl	$6, %eax
	jne	.L46
.L45:
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	jmp	.L47
.L46:
	movl	lstate(%rip), %eax
	cmpl	$2, %eax
	je	.L48
	movl	lstate(%rip), %eax
	cmpl	$3, %eax
	jne	.L47
.L48:
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L47:
	movl	$2, dfastate(%rip)
.L40:
	movl	dfastate(%rip), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	enter, .-enter
	.globl	comment
	.type	comment, @function
comment:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$42, -4(%rbp)
	jne	.L51
	movl	$5, dfastate(%rip)
	jmp	.L52
.L51:
	movl	$4, dfastate(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L52
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L52:
	movl	dfastate(%rip), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	comment, .-comment
	.globl	leave
	.type	leave, @function
leave:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$47, -4(%rbp)
	jne	.L55
	movl	$6, dfastate(%rip)
	jmp	.L56
.L55:
	cmpl	$42, -4(%rbp)
	jne	.L57
	movl	$5, dfastate(%rip)
	jmp	.L56
.L57:
	movl	$4, dfastate(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L56
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
.L56:
	movl	dfastate(%rip), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	leave, .-leave
	.globl	left
	.type	left, @function
left:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$47, -4(%rbp)
	jne	.L60
	movl	$3, dfastate(%rip)
	jmp	.L61
.L60:
	call	__ctype_b_loc
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L62
	movl	$1, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	cmpl	$10, -4(%rbp)
	jne	.L61
	movl	nlines(%rip), %eax
	addl	$1, %eax
	movl	%eax, nlines(%rip)
	jmp	.L61
.L62:
	movl	$2, dfastate(%rip)
	movl	nchars(%rip), %eax
	addl	$1, %eax
	movl	%eax, nchars(%rip)
	movl	nwords(%rip), %eax
	addl	$1, %eax
	movl	%eax, nwords(%rip)
.L61:
	movl	dfastate(%rip), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	left, .-left
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.12) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
