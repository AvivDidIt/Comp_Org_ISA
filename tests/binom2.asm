		.word 0x100 7
		.word 0x101 5
		add $sp, $zero, $zero, $zero, 512 			# 0: set $sp = 512
		sub $sp, $sp, $zero, $zero, 3				# 1: adjust stack for 3 items
		sw  $s2, $sp, $zero, $zero, 2 				# 2: save $s2
		sw  $s1, $sp, $zero, $zero, 1 				# 3: save $s1
		sw  $s0, $sp, $zero, $zero, 0 				# 4: save $s0
		lw $a0, $zero, $zero, $zero, 0x100			# 5: $a0=n
		lw $a1, $zero, $zero, $zero, 0x101			# 6: $a1=k
		add $s0, $zero, $zero, $zero, 515			# 7: initialize C[][] 
		add $t0, $zero, $zero, $zero, 0				# 8: $t0=0 //(i)

F1:		branch $zero, $t0, $a0, 2, Ret				# 9: if (i>n) Ret
		add $t1, $zero, $zero, $zero, 0				# A: $t1=0 //(j)
		branch $zero, $t0, $a1, 3, L1				# B: if(i<k) goto to L1
		add $t2, $a1, $zero, $zero, 0				# C: $t2=k //(min)
		branch $zero, $zero, $zero, $zero, F2			# D: goto F2

L1:		add $t2, $t0, $zero, $zero, 0				# E: min=i

F2:		branch $zero, $t1, $t2, 2, T1				# F: if(j>min) jump to T1 (as in Test1)		
		add $t3, $a1, $zero, $zero, 1				# 10: $t3=k+1
		mac $t3, $t3, $t0, $t1, 0				# 11: $t3=(k+1)*i + j
		add $t3, $t3, $s0, $zero, 0				# 12: $t3=&C[i][j]
		branch $zero, $t1, $zero, 0, IF				# 13: if(j==0) C[i][j]=1
		branch $zero, $t1, $t0, 0, IF				# 14: if(j==i) C[i][j]=1
		branch $zero, $zero, $zero, $zero, ELSE			# 15: jump to ELSE

IF:		add $s1, $zero, $zero, $zero, 1				# 16: $s1=1
		sw $s1, $t3, $zero, $zero, 0				# 17: C[i][j]=1
		branch $zero, $zero, $zero, $zero, T2			# 18: jump to T2 (as in Test2)

ELSE:		sub $s1, $t3, $a1, $zero, 1				# 19: $s1 = &C[i-1][j] = &C[i][j] - (k+1)
		sub $s2, $t3, $a1, $zero, 2				# 1A: $s2 = &C[i-1][j-1] = &C[i][j] - (k+1) - 1
		lw $s1, $s1, $zero, $zero, 0				# 1B: $s1 = C[i-1][j]
		lw $s2, $s2, $zero, $zero, 0				# 1C: $s2 = C[i-1][j-1]
		add $s1, $s1, $s2, $zero, 0				# 1D: $s1 = C[i-1][j] + C[i-1][j-1]
		sw $s1,$t3, $zero, $zero, 0				# 1E: C[i][j] = C[i-1][j] + C[i-1][j-1]

T2:		add $t1, $t1, $zero, $zero, 1				# 1F: j++
		branch $zero, $zero, $zero, $zero, F2			# 20: jump to F2

T1:		add $t0, $t0, $zero, $zero, 1				# 21: i++
		branch $zero, $zero, $zero, $zero, F1			# 22: jump to F1

Ret:		add $t3, $a1, $zero, $zero, 1				# 23: $t3=k+1
		mac $t3, $t3, $a0, $a1, 0				# 24: $t3=(k+1)*n + k
		add $t3, $t3, $s0, $zero, 0				# 25: $t3=&C[n][k]
		lw $t3, $t3, $zero, $zero, 0				# 26: $t3=C[n][k]
		sw $t3, $zero, $zero, $zero, 0x102			# 27: store result in address 0x102
		lw  $s2, $sp, $zero, $zero, 2 				# 28: restore $s2
		lw  $s1, $sp, $zero, $zero, 1 				# 29: restore $s1
		lw  $s0, $sp, $zero, $zero, 0 				# 2A: restore $s0
		add $sp, $sp, $zero, $zero, 3				# 2B: pop 3 items from stack
