	.equ inkey, 1
	.equ PRINT_STR, 8
	.data
text1:	.asciiz "Key pressed \n"
	.text
	.globl main
main:
	# c -> t0
	la $t1, '\n'
while:
	
do:	li $v0, inkey
	syscall
	
	move $t0, $v0	# c = inkey()
	beq $t0, $0, do

if:
	beq $t0, $t1, break	

continue:
	li $v0, PRINT_STR
	la $a0, text1
	syscall

	j while

break:
	li $v0, 0
	jr $ra
