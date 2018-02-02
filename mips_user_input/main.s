 
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
	main_loop0_eif0_001:			.asciiz "Cross is down!\n"
	main_loop0_eif1_001:			.asciiz "Cross is up!\n"
	main_loop0_eif2_001:			.asciiz "Circle is down!\n"
	main_pspDebugScreenPrintf_001:	.asciiz "%d,%d\n"


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
	
	li $a0, 0
	jal sceCtrlSetSamplingCycle			# set sampling cycle
	nop
	
	li $a0, 1							# set sampling mode PSP_CTRL_MODE_ANALOG = 1
	jal sceCtrlSetSamplingMode
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
		
		jal pollPad
		nop
		jal pollLatch
		nop
		
		lw $t0, (SceCtrlLatch)
		andi $t0, $t0, 0x004000
		beq $t0, $0, .e_if0
			la $a0, main_loop0_eif0_001
			jal pspDebugScreenPrintf
			nop
		.e_if0:
		
		lw $t0, (SceCtrlLatch)+4
		andi $t0, $t0, 0x004000
		beq $t0, $0, .e_if1
			la $a0, main_loop0_eif1_001
			jal pspDebugScreenPrintf
			nop
		.e_if1:
		
		lw $t0, (SceCtrlLatch)+8
		andi $t0, $t0, 0x002000
		beq $t0, $0, .e_if2
			la $a0, main_loop0_eif2_001
			jal pspDebugScreenPrintf
			nop
		.e_if2:
		
		la $a0, main_pspDebugScreenPrintf_001
		lb $a1, (SceCtrlData)+8
		lb $a2, (SceCtrlData)+9
		jal pspDebugScreenPrintf
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
