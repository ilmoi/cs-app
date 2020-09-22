/*
void bubble_b(long *data, long count) {
    //reimplementation of the above but using pointers
    long i, j;
    for (i=0; i<count; i++) {
        for (j=0; j<i; j++)
            if (*(data+j+1) < *(data+j)) {
                long t = *(data+j+1);
                *(data+j+1) = *(data+j);
                *(data+j) = t;
            }
    }
}
*/

# ------------------------------------------------------------------------------
#before the function

#need to create stack + allocate space
push %rbp
mov %rsp, %rbp
sub 64 %rsp
#then put the array there
mov $1, -40(%rsp)
mov $2, -32(%rsp)
mov $3, -24(%rsp)
mov $4, -16(%rsp)
mov $5, -8(%rsp)

#need to fill in rdi, rsi = 1st, 2nd arg
leal -40(%rsp), %rdi #array start
mov $5, %rsi #count

#then call the func
call bubble_b

# ------------------------------------------------------------------------------

#setup bp, sp
push %rbp
mov %rsp, %rbp

#check if count <= 0 -> move to end
test %rdi, %rdi
jle .End

#outer loop
.O1
#initialize i
xor %rdx, %rdx #i
#jump to .I1
jmp .I1 #we skip the first increment and immediately jump to the inner loop

.O2
#add 1 to it
inc %rdx
#check if >= count
cmp %rdi, %rdx #rdx-rdi
#if yes - end
jge .End
#initialize j
xor %rcx, %rcx #j
jmp .I3 #we skip the first increment and immediately jump to comparison

#inner loop
.I1
#check if j >= i
cmp %rcx, %rdx #rcx - rdx = j - i
#if yes - jump to .O2
jge .O2
.I2
# do the increment for j
inc %rcx
.I3
#do the swapping
#store the 2 values into 2 registers
mov (%rdi, %rcx, 8), %rax
mov 8(%rdi, %rcx, 8), %rbx
#compare them
cmp %rbx, %rax #j - (j+1)
#if j >= j+1, jump to I1
jge .I1
#else write registers to stack but backwards
mov %rbx, (%rdi, %rcx, 8)
mov %rax, 8(%rdi, %rcx, 8)
jmp .I2

.End
#if it returned the array we'd do
mov %rdi %rax
pop %rbp
ret

# ------------------------------------------------------------------------------
# modify the above so that we use cmov instructions instead of jumps
# max 3 conditional moves
# ok well the places I would put them are:
# 1 i < count
# 2 j < i
# 3 j+1 < j