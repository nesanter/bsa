.text
.global _general_exception_handler
.ent _general_exception_handler

_general_exception_handler:
    la $t0, boot_exception_context
    sw $gp, ($t0)
    sw $sp, 4($t0)
    sw $fp, 8($t0)
    lw $gp, 12($t0)
    lw $sp, 16($t0)
    
    addiu $sp, $sp, -16
    sw $ra, 12($sp)

    srl $a0, $a0, 2
    andi $a0, $a0, 31

    addi $t0, $zero, 8
    bne $a0, $t0, 0f
    nop

    mfc0 $a0, $14
    add $a1, $k1, $zero /* k0 is changed by exception, k1 is not */

    jal boot_syscall_handler
    nop
    
    mtc0 $v0, $14

    lw $ra, 12($sp)

    la $t0, boot_exception_context
    lw $gp, ($t0)
    lw $sp, 4($t0)
    lw $fp, 8($t0)

    jr $ra
    nop

0:
    mfc0 $a2, $14

    jal boot_exception_handler
    nop
    
    j .
    nop

/*
.set noat
_general_exception_handler:
    la $k0, u_exception_context
    sw $gp, ($k0)
    sw $sp, 4($k0)
    sw $fp, 8($k0)
    lw $gp, 12($k0)
    lw $sp, 16($k0)

    move $fp, $sp

    addi $sp, $sp, -104
    sw $31, ($sp)
    sw $25, 4($sp)
    sw $24, 8($sp)
    sw $23, 12($sp)
    sw $22, 16($sp)
    sw $21, 20($sp)
    sw $20, 24($sp)
    sw $19, 28($sp)
    sw $18, 32($sp)
    sw $17, 36($sp)
    sw $16, 40($sp)
    sw $15, 44($sp)
    sw $14, 48($sp)
    sw $13, 52($sp)
    sw $12, 56($sp)
    sw $11, 60($sp)
    sw $10, 64($sp)
    sw $9, 68($sp)
    sw $8, 72($sp)
    sw $7, 76($sp)
    sw $6, 80($sp)
    sw $5, 84($sp)
    sw $4, 88($sp)
    sw $3, 92($sp)
    sw $2, 96($sp)
    sw $1, 100($sp)

    mfc0 $a0, $13
    srl $a0, 2
    andi $a0, $a0, 31
    mfc0 $a1, $14
    addi $t0, $a1, 4
    mtc0 $t0, $14

    jal u_exception_handler

    lw $31, ($sp)
    lw $25, 4($sp)
    lw $24, 8($sp)
    lw $23, 12($sp)
    lw $22, 16($sp)
    lw $21, 20($sp)
    lw $20, 24($sp)
    lw $19, 28($sp)
    lw $18, 32($sp)
    lw $17, 36($sp)
    lw $16, 40($sp)
    lw $15, 44($sp)
    lw $14, 48($sp)
    lw $13, 52($sp)
    lw $12, 56($sp)
    lw $11, 60($sp)
    lw $10, 64($sp)
    lw $9, 68($sp)
    lw $8, 72($sp)
    lw $7, 76($sp)
    lw $6, 80($sp)
    lw $5, 84($sp)
    lw $4, 88($sp)
    lw $3, 92($sp)
    lw $2, 96($sp)
    lw $1, 100($sp)

    la $k0, u_exception_context
    lw $fp, 8($k0)
    lw $sp, 4($k0)
    lw $gp, ($k0)

    eret
    nop
*/
.end _general_exception_handler

.bss
.global u_exception_context
u_exception_context:
.space 20


