	lw	1,2,15	#invalid Opcode in Line 30
	add	0,1,2
	lui	11,2
	sub	1,2,3
	beq		3,2,3
	slt	2,3,1
	or	3,2,5
	addi	3,3,2
	nand	4,5,2
	beq		11,12,15
	slti	3,1,2
	ori	1,8,2
	nand	2,2,2
	jalr	4,5
	sw	10,10,10
	slt	1,2,11
	add	1,2,2
	j	11
	halt
	ori	12,2,12
	sub	10,2,2
	addi	4,6,5
	or	3,2,10
	slti	8,7,6
	jalr	14,10
	lw	0,1,14
	sw	0,1,13
	lui	0,10
	sub	1,2,2
	invalidOp	12,2,4
	halt