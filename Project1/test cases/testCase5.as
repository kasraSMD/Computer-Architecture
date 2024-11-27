	add	3,3,3	#correct test case
	sub	1,2,3
	slt	2,3,1
	or	1,2,5
	nand	1,5,2
	addi	3,3,2
	slti	2,2,10
	ori	1,1,2
	lui	10,2
	lw	1,2,9
	sw	8,8,8
	beq		0,0,-1
	jalr	5,5
	j	lbl1
	halt	0,1,2
lbl1	.fill	5
lbl2	.fill	lbl1
lbl3	.fill	-2