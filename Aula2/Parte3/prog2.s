  .equ CALIBRATION_VALUE, 10000
  .equ SFR_BASE_HI, 0xBF88			#16 Msbits of SFR area
  .equ TRISE, 0x6100				#TRISE address is 0xBF886100
  .equ PORTE, 0x6110				#PORTE address is 0xBF886110
  .equ LATE, 0x6120				#LATE address is 0xBF886120

  .data
  .text
  .globl main

main:

  addiu $sp, $sp, -16
  sw $s0, 0($sp)
  sw $s1, 4($sp)
  sw $s3, 8($sp)
  sw $ra, 12($sp)

  li $s0, 0 # s0 -> v
  lui $s1, SFR_BASE_HI
  lw $s3, TRISE($s1)
  andi $s3, $s3, 0xFFEF # saida => 0
  sw $s3, TRISE($s1)

while:

  lw $s3, LATE($s1)
  andi $s3, $s3, 0xFFEF
  andi $s0, $s0, 0x10
  or $s3, $s0, $s3
  sw $s3, LATE($s1)

  li $a0, 1
  jal delay

  xor $s0, $s0, 0x10

  j while

  lw $ra, 12($sp)
  lw $s0, 0($sp)
  lw $s1, 4($sp)
  lw $s3, 8($sp)
  addiu $sp, $sp, 16

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
