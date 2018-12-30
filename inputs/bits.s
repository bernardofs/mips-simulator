main:
	addiu	$a0, $zero, 37
	jal	no_of_bits
	
	li	$v0, 10			# exit
	syscall
	
## Returns the number of bits necessary to represent an unsigned number.
# $a0	The unsigned number we'll calculate
# $v0	The ammount
no_of_bits:
	add	$t0, $zero, $zero
	
no_of_bits_test:
	beq	$a0, $zero, no_of_bits_end
	srl	$a0, $a0, 1
	addi	$t0, $t0, 1
	j	no_of_bits_test
	
no_of_bits_end:	
	add	$v0, $zero, $t0
	jr	$ra