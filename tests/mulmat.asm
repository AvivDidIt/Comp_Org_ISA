# # store the variables of the matrices in the memory

	.word 0x100 1
	.word 0x101 5
	.word 0x102 -1
	.word 0x103 -2
	.word 0x104 0x00000001
	.word 0x105 0x0000005
	.word 0x106 0xFFFFFFFF
	.word 0x107 0xFFFFFFFE
	.word 0x108 0x000001
	.word 0x109 0x00005
	.word 0x10A -1
	.word 0x10B -2
	.word 0x10C 0x0001
	.word 0x10D 0x005
	.word 0x10E -1
	.word 0x10F -2

# # the second matrix is the Identity matrix

	.word 0x110 1
	.word 0x111 0
	.word 0x112 0
	.word 0x113 0
	.word 0x114 0x0
	.word 0x115 0x1
	.word 0x116 0
	.word 0x117 0
	.word 0x118 0x00
	.word 0x119 0
	.word 0x11A 0x01
	.word 0x11B 0
	.word 0x11C 0x000
	.word 0x11D 0
	.word 0x11E 0
	.word 0x11F 0x001

main:
	jal $zero, $zero, $zero, $zero, mulmat	#1:  jump to function mulmat
	halt $zero, $zero, $zero, $zero, 0	#2:  halt


mulmat:
	add $sp,$zero,$zero,$zero,0x200		#3:  allocate memory for stack at 512
	sub $sp,$zero,$zero,$zero,4		#4:  make room in stack for 4 variables
	sw  $s0,$sp,$zero,$zero,3		#5:  assign address for $s0 (i)
	sw  $s1,$sp,$zero,$zero,2		#6:  assign address for $s1 (j)
	sw  $s2,$sp,$zero,$zero,1		#7:  assign address for $s2 (k)
	sw  $ra,$sp,$zero,$zero,0		#8:  assign address for $ra
	add $t0,$zero,$zero,$zero,4		#9:  $t0 = 4
	add $s0,$zero,$zero,$zero,0		#10: $s0 (i) = 0
	add $t1,$zero,$zero,$zero,0		#11: $t1 (temp) = 0
	
i_loop:
	add $s1,$zero,$zero,$zero,0		#12: $s1 (j) = 0
j_loop:
	add $s2,$zero,$zero,$zero,0		#13: $s2 (k) = 0
k_loop:
	add $t2,$s0,$zero,$zero,0		#14: $t2 = i
	sll $t2,$t2,$zero,$zero,2		#15: $t2 = 4i
	add $t2,$t2,$s2,$zero,0			#16: $t2 = 4i + k
	lw  $t2,$t2,$zero,$zero,0x100		#17: $t2 = MatA[4i + k] = 0x100 + 4i + k
	add $t3,$s2,$zero,$zero,0		#18: $t3 = k
	sll $t3,$t3,$zero,$zero,2		#19: $t3 = 4k
	add $t3,$t3,$s1,$zero,0			#20: $t3 = 4k + j
	lw  $t3,$t3,$zero,$zero,0x110		#21: $t3 = MatB[4k + j] 
	mac $t1,$t2,$t3,$t1,0			#22: $t1 = $t1 + MatA[4i + k] * MatB[4k + j]
	add $s2,$s2,$zero,$zero,1		#23: $s2++	
	branch $zero,$s2,$t0,3,k_loop		#24: if ($s2 < 4): go to k_loop
	add $t3,$s0,$zero,$zero,0		#25: $t3 = i
	sll $t3,$t3,$zero,$zero,2		#26: $t3 = 4i
	add $t3,$t3,$s1,$zero,0			#27: $t3 = 4i + j
	sw  $t1,$t3,$zero,$zero,0x120		#28: store value of $t1 to address 0x120 + 4i + j
	add $t1,$zero,$zero,$zero,0		#29: $t1 (temp) = 0
	add $s1,$s1,$zero,$zero,1		#30: $s1++
	branch $zero,$s1,$t0,3,j_loop		#31: if ($s1 < 4) go to j_loop
	add $s0,$s0,$zero,$zero,1		#32: $s0++
	branch $zero,$s0,$t0,3,i_loop		#33: if ($s0 < 4) go to i_loop
	lw  $s0,$sp,$zero,$zero,3		#34: assign address for $s0 (i)
	lw  $s1,$sp,$zero,$zero,2		#35: assign address for $s1 (j)
	lw  $s2,$sp,$zero,$zero,1		#36: assign address for $s2 (k)
	lw  $ra,$sp,$zero,$zero,0		#37: assign address for $ra
	add $sp,$zero,$zero,$zero,4		#38: return stack
	jr  $ra,$zero,$zero,$zero,0		#39: back to caller

	
	
	
