main_entry
	xor x4 x4 x4
	alloca x1 160000
	alloca x2 160000
	b main_label_1
main_label_1
	muli x5 x4 400
	xor x7 x7 x7
	b main_label_4
main_label_2
	init 400 400
	b draw_label_1
draw_label_1
	xor x11 x11 x11
	b draw_label_2
draw_label_2
	xor x12 x12 x12
	muli x14 x11 400
	b draw_label_5
draw_label_3
	b draw_label_6
draw_label_4
	incEq x13 x11 400
	brCond x13 draw_label_3 draw_label_2
draw_label_5
	add x15 x14 x12
	load x16 x1 x15
	setPixel x12 x11 x16
	incEq x13 x12 400
	brCond x13 draw_label_4 draw_label_5
draw_label_6
	display
	b main_label_5
main_label_3
	incEq x13 x4 400
	brCond x13 main_label_2 main_label_1
main_label_4
	generate x8
	modi x8 x8 2
	add x9 x7 x5
	store x1 x9 x8
	incEq x13 x7 400
	brCond x13 main_label_3 main_label_4
main_label_5
	xor x11 x11 x11
	b calc_label_1
calc_label_1
	xor x12 x12 x12
	muli x14 x11 400
	addi x15 x11 399
	modi x15 x15 400
	muli x15 x15 400
	addi x16 x11 400
	modi x16 x16 400
	muli x16 x16 400
	addi x17 x11 401
	modi x17 x17 400
	muli x17 x17 400
	b calc_label_4
calc_label_2
	b calc_label_5
calc_label_3
	incEq x13 x11 400
	brCond x13 calc_label_2 calc_label_1
calc_label_4
	xor x20 x20 x20
	add x18 x12 x14
	load x19 x1 x18
	icmpEq x20 x19 1
	sextFromBool x37 x20
	addi x22 x12 399
	modi x22 x22 400
	add x23 x15 x22
	load x23 x1 x23
	add x37 x23 x37
	addi x24 x12 400
	modi x24 x24 400
	add x25 x15 x24
	load x25 x1 x25
	add x37 x25 x37
	addi x26 x12 401
	modi x26 x26 400
	add x27 x15 x26
	load x27 x1 x27
	add x37 x27 x37
	add x28 x16 x22
	load x28 x1 x28
	add x37 x28 x37
	add x29 x16 x24
	load x29 x1 x29
	add x37 x29 x37
	add x30 x16 x26
	load x30 x1 x30
	add x37 x30 x37
	add x31 x17 x22
	load x31 x1 x31
	add x37 x31 x37
	add x32 x17 x24
	load x32 x1 x32
	add x37 x32 x37
	add x33 x17 x26
	load x33 x1 x33
	add x37 x33 x37
	icmpEq x34 x37 3
	icmpEq x35 x37 2
	selectFalse x35 x35 x20
	or x35 x35 x34
	store x2 x18 x35
	incEq x13 x12 400
	brCond x13 calc_label_3 calc_label_4
calc_label_5
	b draw_label_7
draw_label_7
	xor x11 x11 x11
	b draw_label_8
draw_label_8
	xor x12 x12 x12
	muli x14 x11 400
	b draw_label_11
draw_label_9
	b draw_label_12
draw_label_10
	incEq x13 x11 400
	brCond x13 draw_label_9 draw_label_8
draw_label_11
	add x15 x14 x12
	load x16 x2 x15
	setPixel x12 x11 x16
	incEq x13 x12 400
	brCond x13 draw_label_10 draw_label_11
draw_label_12
	display
	swap x1 x2
	b main_label_5
