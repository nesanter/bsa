	.extern __vector_dispatch_0
	.section	.vector_0,code
	.align	2
	.set	nomips16
	.ent	__vector_dispatch_0
__vector_dispatch_0:
    la $k0, __vector_table
    lw $k0, 0($k0)
	jr $k0
	.end	__vector_dispatch_0
	.size	__vector_dispatch_0, .-__vector_dispatch_0

