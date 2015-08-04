/*
.align 2
    .set nomips16
    .ent boot_transfer_handler
boot_transfer_handler:
    sw $sp, handler_old_sp
    la $sp, handler_stack
    mfc0 $k1, c0_epc
    mfc0 $k0, c0_srsctl
    sw $k1, 28($sp)
    mfc0 $k1, c0_status
    sw $k0, 24($sp)
    mfc0 $k0, c0_cause
    sw $k1, 20($sp)
    srl $k0, $k0, 0xA
    ins $k1, $k0, 0xA, 0x6
    ins $k1, $zero, 0x1, 0x4
    mtc0 $k1, c0_status
    sw $v1, 8($sp)
    sw $v0, 4($sp)
    lw $v1, 24($sp)
    andi $v1, $v1, 0xf
    sw $s8, 12($sp)
    move $s8, $sp
    li $v0, 1
    sw $v0, -32748($gp)
    move $sp, $s8
    lw $v0, 24($sp)
    andi $v0, $v0, 0xF
    lw $s8, 12($sp)
    lw $v1, 8($sp)
    lw $v0, 4($sp)
    di
    ehb
    lw $k0, 28($sp)
    lw $k1, 28(sp)
    addiu sp, sp, 32

*/
