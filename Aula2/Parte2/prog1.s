  .equ SFR_BASE_HI, 0xBF88			#16 Msbits of SFR area
  .equ TRISE, 0x6100				#TRISE address is 0xBF886100
  .equ PORTE, 0x6110				#PORTE address is 0xBF886110
  .equ LATE, 0x6120				#LATE address is 0xBF886120

  .text
  .globl main

main:
  lui $t2, SFR_BASE_HI
  lw $t3, TRISE($t2)
  andi $t3, $t3, 0xFFF0 # saida => 0
  ori $t3, $t3, 0x00C0 # RE7 e RE6
  sw $t3, TRISE($t2)

  li $t4, 0 #contador binario
  li $v0, 12
  syscall #timer reset

while:
  li $v0, 11
  syscall
  blt $v0, 5000000, while

  lw $t3, PORTE($t2)
  andi $t4, $t3, 0x0040
  andi $t6, $t3, 0x0080
  beqz $t4, contador_binario
  bne $t4, $0, contador_johnson

contador_binario:
  beqz $t6, contador_binario_decrescente
  bnez $t6, contador_binario_crescente

contador_binario_crescente:
  addi $t5, $t5, 1
  andi $t5, $t5, 0x0F

j show

contador_binario_decrescente:
    addi $t5, $t5, -1
    andi $t5, $t5, 0x0F

  j show

contador_johnson:
  beqz $t6, contador_johnson_decrescente
  bnez $t6, contador_johnson_crescente

contador_johnson_crescente:
  beq $t5, 15, reset
  sll $t5,$t5,1
  addi $t5, $t5, 1
  andi $t5, $t5, 0x0F

  j show

reset:
  li $t5, 0
  j show


contador_johnson_decrescente:
  beq $t5, $0, up
  sll $t5,$t5,1
  andi $t5, $t5, 0x0F

j show

up:
  li $t5, 15
  j show


show:

  lw $t3, LATE($t2)
  andi $t3, $t3, 0xFFF0
  or $t3, $t3, $t5
  sw $t3, LATE($t2)

  li $v0, 12
  syscall
  j while

  jr $ra
