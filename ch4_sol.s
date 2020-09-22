# long ys_bubble_p(long *data, long *end)
# data in %rdi, end in %rsi
ysBubbleP:
  jmp L2
L4:
  mrmovq 8(%rax), %r9 #move rax-8 into r9
  mrmovq (%rax), %r10 #move rax into r10
  rrmovq %r9, %r8 #move r9 into r8
  subq %r10, %r8 #r8-r10
  jge L3 #if r8>r10 then we jump
  rmmovq %r10, 8(%rax) #this is the swapping
  rmmovq %r9, (%rax)
L3:
  irmovq $8, %r8
  addq %r8, %rax
  jmp L5
L6:
  rrmovq %rdi, %rax
L5:
  rrmovq %rsi, %r8
  subq %rax, %r8
  jg L4
  irmovq $8, %r8
  subq %r8, %rsi
L2:
  rrmovq %rsi, %r8 #move end into r8
  subq %rdi, %r8 #data - end
  jg L6 #if greater than - we're done
    ret