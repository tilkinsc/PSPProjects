
.data
	.globl SceCtrlData
SceCtrlData: .space 16
	.globl SceCtrlLatch
SceCtrlLatch: .space 16

.text
	.globl pollPad
	.type pollPad, @function
pollPad:
	addiu $sp, $sp, -4				# save main $ra
	sw $ra, ($sp)
	
	la $a0, SceCtrlData
	li $a1, 1
	jal sceCtrlReadBufferPositive
	nop
	
	lw $ra, ($sp)
	addiu $sp, $sp, 4
	jr $ra
	nop


	.globl pollLatch
	.type pollLatch, @function
pollLatch:
	addiu $sp, $sp, -4				# save main $ra
	sw $ra, ($sp)
	
	la $a0, SceCtrlLatch
	jal sceCtrlReadLatch
	nop
	
	lw $ra, ($sp)
	addiu $sp, $sp, 4
	jr $ra
	nop

