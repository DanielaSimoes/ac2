	.equ PRINT_STR, 8
	.equ PRINT_INT, 6
	.equ PRINT_INT10, 7
	.equ READ_INT10, 5
	.data
text1:	.asciiz "\nIntroduza um numero (sinal e modulo):"
text2: 	·asciiz "\nValor lido em base 2:"
text3:	.asciiz "\nVlaor lido em base 16:"
text4: 	.asciiz "\nValor lido em base 10 (unsigned):"
text5: 	.asciiz "\nValor lido em base 10 (signed):
	.text
	.globl main

main:
	# $t0 -> value

while:
	li $v0, PRINT_STR
	la $a0, text1
	syscall

	li $v0, READ_INT10
	syscall

	move $t0, $v0

	li $v0, PRINT_STR
	la $a0, text2
	syscall

	move $a0, $t0
	li $a1, 2
	li $v0, PRINT_INT
	syscall

	li $v0, PRINT_STR
	la $a0, text3
	syscall

	move $a0, $t0
	li $a1, 16
	li $v0, PRINT_INT
	syscall

	li $v0, PRINT_STR
	la $a0, text4
	syscall

	move $a0, $t0
	li $a1, 10
	li $v0, PRINT_INT
	syscall

	li $v0, PRINT_STR
	la $a0, text5
	syscall

	move $a0, $t0
	li $v0, PRINT_INT10
	syscall

	j while
