	.equ getChar, 2
	.equ putChar, 3
	.data
	.text
	.globl main
main: 
	# $t0 -> c
while:
	li $v0, getChar
	syscall
	move $t0, $v0
if: 	
	la $t1, '\n'
	beq $t0, $t1, break

	li $v0, putChar
	move $a0, $t0
	syscall
	
	j while

break: 
	li $v0, 1
	jr $ra	
	 
