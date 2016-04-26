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
andi $t2, $t2, 0xFFFE
ori $t2, $t2, 0x0040
sw $t2, TRISE($t1) #saída

for:

lw $t2, PORTE($t1)
andi $t2, $t2, 0x0040
srl $t2, $t2, 6

lw $t3, LATE($t1)
andi $t3, $t3, 0xFFFE
nor $t3, $t3, $t2
sw $t3, LATE($t1)

j for

jr $ra
