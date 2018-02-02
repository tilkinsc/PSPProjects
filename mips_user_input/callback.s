
.data
	callback_exitRequest:	.word	1
	callback_sceKernelCreateCallback_001:	.asciiz	"Exit Callback"
	callback_sceKernelCreateThread_001:		.asciiz	"Callback Update Thread"

.text
	.globl	isRunning
	.type isRunning, @function
isRunning:
	la $t0, callback_exitRequest
	lw $v0, ($t0)
	jr $ra
	nop


	.globl	exitCallback
	.type exitCallback, @function
exitCallback:
	la $t0, callback_exitRequest
	li $t1, 0
	sw $t1, ($t0)
	li $v0, 0
	jr $ra
	nop


	.globl	callbackThread
	.type callbackThread, @function
callbackThread:
	addiu $sp, $sp, -4		# retain ra
	sw $ra, ($sp)
	
	la $t0, callback_sceKernelCreateCallback_001
	lw $a0, ($t0)
	la $t0, exitCallback
	lw $a1, ($t0)
	li $a2, 0
	jal sceKernelCreateCallback
	nop
	move $a0, $v0
	jal sceKernelRegisterExitCallback
	nop
	jal sceKernelSleepThreadCB
	nop
	
	lw $ra, ($sp)			# pop $ra from stack
	addiu $sp, $sp, 4
	jr $ra
	nop
	
	
	.globl	setupExitCallback
	.type setupExitCallback, @function
setupExitCallback:
	addiu $sp, $sp, -4		# retain ra
	sw $ra, ($sp)
	
	la $t0, callback_sceKernelCreateThread_001
	lw $a0, ($t0)
	la $t0, callbackThread
	lw $a1, ($t0)
	li $a2, 0x11
	li $a3, 0xFA0
	li $t0, 0x80000000
	li $t1, 0
	sw $t0, 16($fp)
	sw $t1, 20($fp)
	jal sceKernelCreateThread	# have to load some variables to stack
	nop
	
	blez $v0, .nullptrts
	move $a0, $v0
	jal sceKernelStartThread
	nop
.nullptrts:
	lw $ra, ($sp)			# pop $ra from stack
	addiu $sp, $sp, 4
	jr $ra
	nop
