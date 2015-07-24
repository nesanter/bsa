.text

.set noat

.global context_switch
.ent context_switch

/* arguments: a0 -- save context
              a1 -- restore context
              a2 -- return value
              a3 -- exit address

   this function stores the callee-preserved
   registers in the context pointed to by save context
   (if save context is zero, this step is skipped)
   it then loads a new context (aka set of callee-
   preserved registers) from the restore context

   finally, a return address is loaded from restore
   context and jumped to.

   if exit_address is non-zero, spoof the return address
   into being exit_address and then jump to the return address
   loaded from the restore context
*/

context_switch:
    beq a0, zero, Lskip_save

    sw s0, (a1)
    sw s1, 4(a1)
    sw s2, 8(a1)
    sw s3, 12(a1)
    sw s4, 16(a1)
    sw s5, 20(a1)
    sw s6, 24(a1)
    sw s7, 28(a1)
    sw gp, 32(a1)
    sw sp, 36(a1)
    sw fp, 40(a1)
    sw ra, 44(a1)

Lskip_save:

    bnz a3, Lentry

    lw s0, (a2)
    lw s1, 4(a2)
    lw s2, 8(a2)
    lw s3, 12(a2)
    lw s4, 16(a2)
    lw s5, 20(a2)
    lw s6, 24(a2)
    lw s7, 28(a2)
    lw gp, 32(a1)
    lw sp, 36(a1)
    lw fp, 40(a1)
    lw ra, 44(a1)

    add v0, a2, zero
    
    jr ra

Lentry:

    lw sp, 36(a1)
    lw t0, 44(a1)

    add ra, a3, zero
    jr t0

.end context_switch