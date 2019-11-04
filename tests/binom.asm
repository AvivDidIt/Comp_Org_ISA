	.word 0x100 10	
	.word 0x101 7
main:
	add $sp, $zero, $zero, $zero, 512		# 0: set $sp = 512
	lw $a0, $zero, $zero, $zero, 0x100		# 1: get n from address 0x100
	lw $a1, $zero, $zero, $zero, 0x101		# 2: get k from address 0x101
	jal $zero, $zero, $zero, $zero,binom		# 3: calc $v0 = binom(n,k)
	sw $v0, $zero, $zero, $zero, 0x102		# 4: store fib(x) in 257
	halt $zero, $zero, $zero, $zero, 0		# 5: halt


binom:
	sub $sp, $sp, $zero, $zero, 4			# 6: adjust stack for 4 items
	sw $s0, $sp, $zero, $zero, 3			# 7: save $s0
	sw $ra, $sp, $zero, $zero, 2			# 8: save return address
	sw $a1, $sp, $zero, $zero, 1			# 9: save argument
	sw $a0, $sp, $zero, $zero, 0			# 10: save argument
	branch $zero, $a0, $a1,0, Ret			# 11: jump to Ret if n=k
	branch $zero, $a1, $zero,0, Ret			# 12: jump to Ret if k=0
L1:
	sub $a0, $a0, $zero, $zero, 1			# 13: calculate n - 1
	sub $a1, $a1, $zero, $zero, 1			# 14: calculate k - 1
	jal $zero, $zero, $zero, $zero,binom		# 15: calc $v0=binom(n-1,k-1)
	add $s0, $v0, $zero, $zero, 0			# 16: $s0 = binom(n-1,k-1)
	lw $a0, $sp, $zero, $zero, 0			# 17: restore $a0 = n
	lw $a1, $sp, $zero, $zero, 1			# 18: restore $a1 = k
	sub $a0, $a0, $zero, $zero, 1			# 19: calculate n - 1
	jal $zero, $zero, $zero, $zero, binom		# 20: calc binom(n-1,k)
	add $v0, $v0, $s0, $zero, 0			# 21: $v0 = binom(n-1,k-1) + binom(n-1,k)
	lw $a0, $sp, $zero, $zero, 0			# 22: restore $a0
	lw $a1, $sp, $zero, $zero, 1			# 23: restore $a1
	lw $ra, $sp, $zero, $zero, 2			# 24: restore $ra
	lw $s0, $sp, $zero, $zero, 3			# 25: restore $s0
	add $sp, $sp, $zero, $zero, 4			# 26: pop 4 items from stack
	jr $ra, $zero, $zero, $zero, 0			# 27: and return

Ret:
	add $v0, $zero, $zero, $zero, 1			# 28: otherwise, binom = 1
	add $sp, $sp, $zero, $zero, 4			# 29: pop 3 items from stack
	jr $ra, $zero, $zero, $zero, 0			# 30: and return	

