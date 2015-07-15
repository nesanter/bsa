	.extern __vector_dispatch_31
	.section	.vector_31,code
	.align	2
	.set	nomips16
	.ent	__vector_dispatch_31
__vector_dispatch_31:
	j	boot_transfer_handler
	.end	__vector_dispatch_31
	.size	__vector_dispatch_31, .-__vector_dispatch_31

