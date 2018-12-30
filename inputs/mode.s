#Pastrolin Alberto
#12/12/2012

.data
	string: .asciiz "abababab"				#the string that we wont to clean
	n: .word 8							#string length

.text
	li $t0, 0							#the begin of the string
	li $s2, -1							#the flag for a vocal
	lw $s7, n							#the end of the string
	jal __delete						#find and sign every vocal
	
	j __end							#print the result
	
	__delete:
		lb $t1, string($t0)				#read an element from the string
		beq $t1, 65, __flag
		beq $t1, 97, __flag
		beq $t1, 69, __flag
		beq $t1, 101, __flag
		beq $t1, 73, __flag
		beq $t1, 105, __flag
		beq $t1, 79, __flag
		beq $t1, 111, __flag
		beq $t1, 85, __flag
		beq $t1, 117, __flag				#with this jump i check if the element is into the range {a,A,e,E,i,I,o,O,u,U}
		j __go
	__flag:
		sb $s2, string($t0)				
			#if we found a vocal, we replace it with a -1 
			#(because the ASCII is a code that use an unsigned byte,
			# a -1 is impossible for this code, so we can use this number as a flag,
			# cause we can't think that it is a letter of the code)
		move $s0, $ra
		jal __reduce
		move $ra, $s0
	__go:
		addi $t0, $t0, 1				
		blt $t0, $s7, __delete			#we have finished to check?
		jr $ra						#if yes we return back
	
	__reduce:
		move $t4, $t0					#a second index that we use to iterate
		__comprime:
			lb $t1, string($t4)
			beq $t1, -1, __rev			#maybe we have found a flag...
			j __next					#nope :p
		__rev:
			addi $t2, $t4, 1			#the next element of the string
			lb $t3, string($t2)
			sb $t3, string($t4)
			sb $t1, string($t2)			#we exchange the two values in memories
		__next:
			addi $t4, $t4, 1
			blt $t4, $s7, __comprime		#we are arrived to the end of the string
		jr $ra						#and if yes, we have finished :)
		
	__end:
		la $a0, string
		li $v0, 4
		syscall						#print the result
		li $v0, 10
		syscall