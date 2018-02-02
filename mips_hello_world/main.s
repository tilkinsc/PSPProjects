 
# Module Info
	    .set push
    .section .lib.ent.top, "a", @progbits
    .align 2
    .word 0
__lib_ent_top:
    .section .lib.ent.btm, "a", @progbits
    .align 2
__lib_ent_bottom:
    .word 0
    .section .lib.stub.top, "a", @progbits
    .align 2
    .word 0
__lib_stub_top:
    .section .lib.stub.btm, "a", @progbits
    .align 2
__lib_stub_bottom:
    .word 0
		.set pop
.text
	.globl	module_info
	.section	.rodata.sceModuleInfo,"a",@progbits
	.align	4
	.type	module_info, @object
	.size	module_info, 52
module_info:
	.half	0
	.byte	0
	.byte	1
	.ascii	"HelloWorld\000"
	.space	16
	.byte	0
	.word	_gp
	.word	__lib_ent_top
	.word	__lib_ent_bottom
	.word	__lib_stub_top
	.word	__lib_stub_bottom
	.globl	sce_newlib_attribute
	.section	.sdata,"aw",@progbits
	.align	2
	.type	sce_newlib_attribute, @object
	.size	sce_newlib_attribute, 4

# Thread Attribute
sce_newlib_attribute:
	.word	-2147483648		# signed 0xFFFFFFFF PSP_THREAD_ATTR_USER


# Main Program
.data
	main_pspDebugScreenSetXY_001:	.asciiz	"I made this only using assmebly B)"

.text
	.globl	main
	.type main, @function
main:
	addiu $sp, $sp, -4					# save main $ra
	sw $ra, ($sp)
	
	jal setupExitCallback				# callback.h
	nop
	
	jal pspDebugScreenInit				# pspdebug.h
	nop
	
.loop0:	# while loop
		jal isRunning
		nop
		
		bne $v0, 1, .e_loop0 			# break on `isRunning() == 0`
		
		jal sceDisplayWaitVblankStart	# pspdisplay.h
		nop
		
		jal pspDebugScreenClear			# pspdebug.h
		nop
		
		li $a0, 0
		li $a1, 0
		jal pspDebugScreenSetXY			# pspdebug.h
		nop
		
		la $a0, main_pspDebugScreenSetXY_001
		jal pspDebugScreenPrintf		# pspdebug.h
		nop
		
		j .loop0
		nop
.e_loop0:
	jal sceKernelExitGame				# pspkernel.h
	nop
	
	li $v0, 0							# return 0;
	lw $ra, ($sp)
	addiu $sp, $sp, 4					# pop $ra from stack
	jr $ra
	nop
