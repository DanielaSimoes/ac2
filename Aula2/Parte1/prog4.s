.equ SFR_BASE_HI, 0xBF88
.equ TRISE, 0x6100
.equ PORTE, 0x6110
.equ LATE, 0x6120
.data
.text
.globl main

main:
  lui $t1, SFR_BASE_HI
  lw $t2, TRISE($t1)
  andi $t2, $t2, 0xFFF0
  ori $t2, $t2, 0x00C0
  sw $t2, TRISE($t1) #saída

for:

  lw $t2, PORTE($t1)
  andi $t3, $t2, 0x0040 # ler R6
  andi $t4, $t2, 0x0080 # ler R7

  srl $t3, $t3, 6
  srl $t4, $t4, 7

  and $t5, $t3, $t4 # RE6 & RE7
  or $t6, $t3, $t4  # RE6 | RE7
  xor $t7, $t3, $t4  # RE6 ^ RE7
  nor $t8, $t3, $t4  # ~(RE6 | RE7)
  andi $t8,$t8,0x0001

  lw $t9, LATE($t1)
  andi $t9, $t9, 0xFFF0
  or $t9,$t9,$t5
  sll $t6, $t6, 1
  or $t9,$t9,$t6
  sll $t7, $t7, 2
  or $t9,$t9,$t7
  sll $t8, $t8, 3
  or $t9,$t9,$t8
  sw $t9, LATE($t1)

  j for

  jr $ra
