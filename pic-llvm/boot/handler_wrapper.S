    .extern handler_wrapper
    .text
    .align	2
    .set	nomips16
    .ent	handler_wrapper
    .set noat
handler_wrapper:
    beq $k0, $zero, Lskip
    addi $sp, $sp, -80
    sw $1, ($sp)
    sw $2, 4($sp)
    sw $3, 8($sp)
    sw $4, 12($sp)
    sw $5, 16($sp)
    sw $6, 20($sp)
    sw $7, 24($sp)
    sw $8, 28($sp)
    sw $9, 32($sp)
    sw $10, 36($sp)
    sw $11, 40($sp)
    sw $12, 44($sp)
    sw $13, 48($sp)
    sw $14, 52($sp)
    sw $15, 56($sp)
    sw $24, 60($sp)
    sw $25, 64($sp)
    sw $28, 72($sp)
    sw $31, 76($sp)
    jal $k0
    lw $1, ($sp)
    lw $2, 4($sp)
    lw $3, 8($sp)
    lw $4, 12($sp)
    lw $5, 16($sp)
    lw $6, 20($sp)
    lw $7, 24($sp)
    lw $8, 28($sp)
    lw $9, 32($sp)
    lw $10, 36($sp)
    lw $11, 40($sp)
    lw $12, 44($sp)
    lw $13, 48($sp)
    lw $14, 52($sp)
    lw $15, 56($sp)
    lw $24, 60($sp)
    lw $25, 64($sp)
    lw $28, 72($sp)
    lw $31, 76($sp)
    addi $sp, $sp, 80
    eret
Lskip:
    add $k0, $zero, $zero
    jr $k0
    .end    handler_wrapper
    .size	handler_wrapper, .-handler_wrapper

