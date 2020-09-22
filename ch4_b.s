	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 4
	.globl	_bubble_b               ## -- Begin function bubble_b
	.p2align	4, 0x90
_bubble_b:                              ## @bubble_b
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	testq	%rsi, %rsi
	jle	LBB0_8
## %bb.1:
	xorl	%r8d, %r8d
	testq	%r8, %r8
	jne	LBB0_3
	.p2align	4, 0x90
LBB0_7:                                 ## =>This Inner Loop Header: Depth=1
	incq	%r8
	cmpq	%rsi, %r8
	je	LBB0_8
## %bb.2:                               ##   in Loop: Header=BB0_7 Depth=1
	testq	%r8, %r8
	je	LBB0_7
LBB0_3:
	xorl	%ecx, %ecx
	jmp	LBB0_4
	.p2align	4, 0x90
LBB0_6:                                 ##   in Loop: Header=BB0_4 Depth=1
	incq	%rcx
	cmpq	%rcx, %r8
	je	LBB0_7
LBB0_4:                                 ## =>This Inner Loop Header: Depth=1
	movq	(%rdi,%rcx,8), %rax
	movq	8(%rdi,%rcx,8), %rdx
	cmpq	%rax, %rdx
	jge	LBB0_6
## %bb.5:                               ##   in Loop: Header=BB0_4 Depth=1
	movq	%rax, 8(%rdi,%rcx,8)
	movq	%rdx, (%rdi,%rcx,8)
	jmp	LBB0_6
LBB0_8:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	movq	$5, -48(%rbp)
	movq	$4, -40(%rbp)
	movq	$3, -32(%rbp)
	movq	$2, -24(%rbp)
	movq	$1, -16(%rbp)
	leaq	-48(%rbp), %rdi
	movl	$5, %esi
	callq	_bubble_b
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-8(%rbp), %rax
	jne	LBB1_2
## %bb.1:
	xorl	%eax, %eax
	addq	$48, %rsp
	popq	%rbp
	retq
LBB1_2:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
