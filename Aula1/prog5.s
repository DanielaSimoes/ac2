.equ PRINT_STR, 8
.equ READ_STR, 9
.equ PRINT_INT, 6
.equ PRINT_INT10, 7
.equ STR_MAX_SIZE, 20
.data
str1:  .space 21
str2:  .space 21
str3:  .space 41
text1: .asciiz "Introduza duas strings: "
text2: .asciiz "Resultados:\n"
text3: .asciiz "\n"
.text
.globl main
main:

addiu $sp, $sp, -4
sw $ra, 0($sp)

li $v0, PRINT_STR
la $a0, text1
syscall

li $v0, READ_STR
la $a0, str1
li $a1, STR_MAX_SIZE
syscall

li $v0, READ_STR
la $a0, str2
li $a1, STR_MAX_SIZE
syscall

li $v0, PRINT_STR
la $a0, text2
syscall

la $a0, str1
jal strlen
move $a0, $v0
li $a1, 10
li $v0, PRINT_INT
syscall

li $v0, PRINT_STR
la $a0, text3
syscall

la $a0, str2
jal strlen
move $a0, $v0
li $a1, 10
li $v0, PRINT_INT
syscall


li $v0, PRINT_STR
la $a0, text3
syscall

la $a0, str3
la $a1, str1
jal strcpy



la $a0, str3
la $a1, str2
jal strcat
move $a0, $v0
li $v0, PRINT_STR
syscall


li $v0, PRINT_STR
la $a0, text3
syscall

la $a0, str1
la $a1, str2
jal strcmp
move $a0, $v0
li $v0, PRINT_INT10
syscall


li $v0, PRINT_STR
la $a0, text3
syscall

li $v0, 0

lw $ra, 0($sp)
addiu $sp, $sp, 4
jr $ra


strlen:
addiu $sp, $sp, -12
sw $ra, 0($sp)
sw $s0, 4($sp)
sw $s1, 8($sp)

move $s0, $a0 #save input -> char *s
li $s1, 0 #$s1 -> len

for_strlen:
lb $t0, 0($s0)
beqz $t0, return_strlen
addi $s1, $s1, 1
addi $s0, $s0, 1

j for_strlen

return_strlen:
move $v0, $s1
lw $ra, 0($sp)
lw $s0, 4($sp)
lw $s1, 8($sp)
addiu $sp, $sp, 12
jr $ra

strcat:
addiu $sp, $sp, -16
sw $ra, 0($sp)
sw $s0, 4($sp) #*dst
sw $s1, 8($sp) #*src
sw $s2, 12($sp) #*rp

move $s0, $a0
move $s1, $a1
move $s2, $s0

for_strcat:
lb $t0, 0($s0)
beq $t0, $0, endfor_strcat
addi $s0, $s0, 1

j for_strcat

endfor_strcat:
move $a0, $s0
move $a1, $s1
jal strcpy
move $v0, $s2

lw $ra, 0($sp)
lw $s0, 4($sp) #*dst
lw $s1, 8($sp) #*src
lw $s2, 12($sp) #*rp
addiu $sp, $sp, 16

jr $ra

strcpy:
addiu $sp, $sp, -16
sw $ra, 0($sp)
sw $s0, 4($sp) #*dst
sw $s1, 8($sp) #*src
sw $s2, 12($sp) #*rp

move $s0, $a0
move $s1, $a1
move $s2, $s0

for_strcpy:
lb $t0, 0($s1) #*src
sb $t0, 0($s0)
beq $t0, $0, return_strcpy
addi $s0, $s0, 1
addi $s1, $s1, 1
j for_strcpy

return_strcpy:
move $v0, $s2
lw $ra, 0($sp)
lw $s0, 4($sp) #*dst
lw $s1, 8($sp) #*src
lw $s2, 12($sp) #*rp
addiu $sp, $sp, 16
jr $ra

strcmp:
addiu $sp, $sp, -12
sw $ra, 0($sp)
sw $s1, 4($sp) #*s1
sw $s2, 8($sp) #*s2

move $s1, $a0
move $s2, $a1

for_strcmp:
lb $t1, 0($s1)
lb $t2, 0($s2)
bne $t1, $t2, return_strcmp
beq $t1, $0, return_strcmp
addi $s1, $s1, 1
addi $s2, $s2, 1
j for_strcmp

return_strcmp:
lb $t1, 0($s1)
lb $t2, 0($s2)
sub $v0, $t1, $t2
lw $ra, 0($sp)
lw $s1, 4($sp)
lw $s2, 8($sp)
addiu $sp, $sp, 12
jr $ra
