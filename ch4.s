	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 10, 15	sdk_version 10, 15, 4
	.globl	_bubble_a               ## -- Begin function bubble_a
	.p2align	4, 0x90
_bubble_a:                              ## @bubble_a
	.cfi_startproc
## %bb.0:
	pushq	%rbp ## push whatever is inside rbp onto the stack
	.cfi_def_cfa_offset 16 ##ignore
	.cfi_offset %rbp, -16 ##ignore
	movq	%rsp, %rbp ##start a new frame by moving rsp into rbp. Turns out rbp is still used for variable length frames
	.cfi_def_cfa_register %rbp
	testq	%rsi, %rsi #test rsi
	jle	LBB0_8 #if rsi <= zero go to 8th instruction
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
	movq	%rdx, %rcx
	cmpq	%rdx, %r8
	je	LBB0_7
LBB0_4:                                 ## =>This Inner Loop Header: Depth=1
	leaq	1(%rcx), %rdx
	movq	(%rdi,%rcx,8), %r9
	movq	8(%rdi,%rcx,8), %rax
	cmpq	%r9, %rax
	jge	LBB0_6
## %bb.5:                               ##   in Loop: Header=BB0_4 Depth=1
	movq	%r9, 8(%rdi,%rcx,8)
	movq	%rax, (%rdi,%rcx,8)
	jmp	LBB0_6
LBB0_8:
	popq	%rbp #pop stack frame
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_bubble_b               ## -- Begin function bubble_b
	.p2align	4, 0x90

# ------------------------------------------------------------------------------

_bubble_b:                              ## @bubble_b
	.cfi_startproc
## %bb.0:
	pushq	%rbp #new bp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp #new stack frame
	.cfi_def_cfa_register %rbp
	testq	%rsi, %rsi #test rsi, which is 2nd argument, which is 5
	jle	LBB1_8 #if <= 0 then we're done, so we jump to pretty much the end of the function
	# basically if max counter was 0 or lower there'd be no reason to even go into looping
## %bb.1:
	xorl	%r8d, %r8d #set r8d to 0 - this is the low 32 bits of the r8 register. Ok this is i
	testq	%r8, %r8 #test r8
	jne	LBB1_3 #if not equal 0, then jump to section 1_3.. weird we'd never jump there?
	.p2align	4, 0x90


# outer loop begins
LBB1_7:
	incq	%r8 #increment r8 by 1, which I'm guessing is i
	cmpq	%rsi, %r8 #compare r8 with rsi, which is the second arg, which is 5
	je	LBB1_8 #if equal we're done.
	# otherwise fall through below
	testq	%r8, %r8 #test r8
	je	LBB1_7 #if equal zero then jump to the start of the loop, so we can increment the first time


# inner loop
LBB1_3: #initialization of inner loop
	xorl	%ecx, %ecx #set ecx to 0 - this will be j
	jmp	LBB1_4 #jump, unconditionally to 1_4
	.p2align	4, 0x90
LBB1_6: #main body of inner loop
	incq	%rcx #increment rcx, which I'm guessing is j
	cmpq	%rcx, %r8 #compare j with i
	je	LBB1_7 #if equal, then we jump to 1_7, which starts with incrementing i, else fall through
	# but because rcx is now +1 we're doing the comparison on next pair of elements
LBB1_4: #comparison + exchange part of inne rloop
	movq	(%rdi,%rcx,8), %rax #rdi (which points to array start) + rcx*8 -> into rax
	movq	8(%rdi,%rcx,8), %rdx #same as above but 8 bits later -> into rdx. So this is the j+1 elem
	cmpq	%rax, %rdx #compare the two
	jge	LBB1_6 #if rdx-rax => 0 jump to 1_6 - EXPECT THIS TO BE THE "DONT DO EXCHANGE" BRANCH
	movq	%rax, 8(%rdi,%rcx,8) #otherwise do the exchange
	movq	%rdx, (%rdi,%rcx,8)
	jmp	LBB1_6


# end of function
LBB1_8:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90

# ------------------------------------------------------------------------------

_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp #new bp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp #new stack frame
	.cfi_def_cfa_register %rbp
	pushq	%r14 #push r14 onto stack
	pushq	%rbx #push rbx onto stack
	subq	$48, %rsp #allocate 48bytes for stack
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax #canary
	movq	(%rax), %rax #canary
	movq	%rax, -24(%rbp) #canary - put it 24 bytes down from rbp
	leaq	L_str(%rip), %rdi #load hello world into rdi
	callq	_puts #call print
	movq	$11, -64(%rbp) #load a bunch of numbers onto the stack
	movq	$2, -56(%rbp)
	movq	$33, -48(%rbp)
	movq	$4, -40(%rbp)
	movq	$55, -32(%rbp) #note how it's going up to 24, where the canary sits
	leaq	-64(%rbp), %rdi #1st argument = 11, which is the first element in the array
	movl	$5, %esi #2nd argument, which is the count = 5
	callq	_bubble_b #call bubble_b
	leaq	L_.str.1(%rip), %r14
	xorl	%ebx, %ebx
	.p2align	4, 0x90
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	movq	-64(%rbp,%rbx,8), %rsi
	movq	%r14, %rdi
	xorl	%eax, %eax
	callq	_printf
	incq	%rbx
	cmpq	$5, %rbx
	jne	LBB2_1
## %bb.2:
	movq	___stack_chk_guard@GOTPCREL(%rip), %rax
	movq	(%rax), %rax
	cmpq	-24(%rbp), %rax
	jne	LBB2_4
## %bb.3:
	xorl	%eax, %eax
	addq	$48, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
LBB2_4:
	callq	___stack_chk_fail
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str.1:                               ## @.str.1
	.asciz	"%ld\n"

L_str:                                  ## @str
	.asciz	"hello world"


.subsections_via_symbols
