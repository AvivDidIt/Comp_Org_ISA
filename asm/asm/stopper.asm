	add			$s0, $zero, $zero, $zero, 6			#s0=6
	add			$s1,$zero,$zero,$zero,10			#s1=10
reset:
	add			$t0,$zero,$zero,$zero,26			#t0=26
	in			$t3,$zero,$zero,$zero,3				#save value of BTND to $t3
	add			$s2, $zero, $zero, $zero, 0			#s2=0 (this will be the main timer register)
	out			$zero,$zero,$zero,$zero,4			#reset IOR[4] (clock = 00:00)
sec_pad:
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
ten_sec_pad:
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
min_pad:
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
ten_min_pad:
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
	add			$zero,$zero,$zero,$zero,0			#padding
main_loop:
	in			$t2,$zero,$zero,$zero,2				#save value of BTNC to $t2
	and			$t2,$t2,$t2,$zero,1					#check if value of BTNC is even or odd
	branch		$zero,$t2,$zero,1,led_on			#if BTND value is odd go to pause
	in			$t2,$zero,$zero,$zero,3				#save value of BTND to $t2
	branch		$zero,$t2,$t3,1,reset				#if new value of BTND != old value of BTND go to reset
	and			$t1,$s2,$s2,$zero,10				#check if sec==10
	branch		$zero,$t1,$s1,0,add_10sec			#if so go to add_10sec 
	out			$s2,$zero,$zero,$zero,4				#update timer to register IOR[4]
	add			$s2,$s2,$zero,$zero,1				#s2++
	branch		$zero,$zero,$zero,0,sec_pad			#go to sec_pad
add_10sec:
	add			$s2,$s2,$zero,$zero,6				#add 6 to main clock register so 00.0A becomes 00.10
	sra			$t1,$s2,$zero,$zero,4				#move 10sec bits to the first 4 bits and save to t1
	and			$t1,$t1,$t1,$zero,0xF				#isolate the first 4 LSBs
	branch		$zero,$t1,$s0,0,add_min				#if this bits == 6 go to add_min
	out			$s2,$zero,$zero,$zero,4				#update timer to register IOR[4]
	add			$s2,$s2,$zero,$zero,1				#s2++
	branch		$zero,$zero,$zero,0,ten_sec_pad		#go to ten_sec_pad
add_min:
	add			$s2,$s2,$zero,$zero,160				#add 160 to main clock register so 00.60 becomes 01.00
	sra			$t1,$s2,$zero,$zero,8				#move min bits to the first 4 bits and save to t1
	and			$t1,$t1,$t1,$zero,0xF				#isolate the first 4 LSBs
	branch		$zero,$t1,$s1,0,add_10min			#if this bits == 10 go to add_10min
	out			$s2,$zero,$zero,$zero,4				#update timer to register IOR[4]
	add			$s2,$s2,$zero,$zero,1				#s2++
	branch		$zero,$zero,$zero,0,min_pad			#go to min_pad
add_10min:
	add			$s2,$s2,$zero,$zero,1536			#add 1536 to main clock register so 09.59 becomes 10.00
	sra			$t1,$s2,$zero,$zero,12				#move 10min bits to the first 4 bits and save to t1
	and			$t1,$t1,$t1,$zero,0xF				#isolate the first 4 LSBs
	branch		$zero,$t1,$s0,0,reset				#if this bits == 6 reset the stopper
	out			$s2,$zero,$zero,$zero,4				#update timer to register IOR[4]
	branch		$zero,$zero,$zero,0,ten_min_pad		#go to main_loop
led_on:
	add			$t0,$zero,$zero,$zero,26			#t0=26
	add			$t1,$zero,$zero,$zero,1				#t1=1
	out			$t1,$zero,$zero,$zero,1				#turn on Led0
pause:
	in			$t2,$zero,$zero,$zero,2				#save value of BTNC to $t2
	and			$t2,$t2,$t2,$zero,1					#check if value of BTNC is even or odd
	branch		$zero,$t2,$zero,0,led_off			#if even go to led_off and stop pause
	sub			$t0,$t0,$zero,$zero,1				#t0--
	branch		$zero,$t0,$zero,0,led_switch		#if t0==0 go to led_switch
	branch		$zero,$zero,$zero,$zero,pause		#else do pause loop again
led_switch:
	branch		$zero,$t1,$zero,0,led_on			#if t1==0 go to led_on
	branch		$zero,$zero,$zero,$zero,led_off		#else go to led_off
led_off:
	add			$t0,$zero,$zero,$zero,26			#t0=20
	sub			$t0,$t0,$zero,$zero,1				#t0--
	add			$t1,$zero,$zero,$zero,0				#t1=0
	out			$zero,$zero,$zero,$zero,1			#turn off led_0
	in			$t2,$zero,$zero,$zero,2				#t2 = value of IOR[2]
	and			$t2,$t2,$t2,$zero,1					#check if even or odd
	branch		$zero,$t2,$zero,0,main_loop			#if even go back to main loop
	branch		$zero,$zero,$zero,$zero,pause		#else go back to pause loop

	
