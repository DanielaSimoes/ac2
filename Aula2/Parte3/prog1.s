  .equ CALIBRATION_VALUE, 39800
  .data
str: 	.asciiz "\r\n"
  .text
  .globl main

main:
  addiu $sp, $sp, -4
  sw $ra, 0($sp)

while:
  li $v0, 12
  syscall #resetCoreTimer();

  li $a0, 1
  jal delay

  li $v0, 11
  syscall

  move $a0, $v0
  li $a1, 10
  sll $a1, $a1, 16
  addi $a1, $a1, 10

  li $v0, 7
  syscall

  la $a0, str
	li $v0, 8
	syscall

  j while

  lw $ra, 0($sp)
  addiu $sp, $sp, 4

  jr $ra

delay:
  # parametros de entrada n_intervals

for_delay:
  beqz $a0, end_for_delay
  li $t0, CALIBRATION_VALUE # t0 -> i = CALIBRATION_VALUE

second_for_delay:
  beqz $t0, end_second_for_delay
  addi $t0, $t0, -1

  j second_for_delay

end_second_for_delay:
  addi $a0, $a0, -1
  j for_delay

end_for_delay:
  jr $ra
