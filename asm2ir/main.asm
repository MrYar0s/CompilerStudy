main_entry
	XOR x4 x4 x4
	ALLOCA x1 160000
	ALLOCA x2 160000
	GET_ELEM_PTRi x3 x1 0
	B main_label_1
main_label_1
	MULi x5 x4 400
	XOR x7 x7 x7
	B main_label_4
main_label_2
	GET_ELEM_PTRi x6 x2 0
	INIT 400 400
	B draw_label_1
draw_label_1
	XOR x11 x11 x11
	B draw_label_2
draw_label_2
	XOR x12 x12 x12
	MULi x14 x11 400
	B draw_label_5
draw_label_3
	B draw_label_6
draw_label_4
	INC_EQ x13 x11 400
	BR_COND x13 draw_label_3 draw_label_2
draw_label_5
	ADD x15 x14 x12
	LOAD x16 x3 x15
	SET_PIXEL x12 x11 x16
	INC_EQ x13 x12 400
	BR_COND x13 draw_label_4 draw_label_5
draw_label_6
	DISPLAY
	B main_label_5
main_label_3
	INC_EQ x13 x4 400
	BR_COND x13 main_label_2 main_label_1
main_label_4
	GENERATE x8
	MODi x8 x8 2
	ADD x9 x7 x5
	STORE x1 x9 x8
	INC_EQ x13 x7 400
	BR_COND x13 main_label_3 main_label_4
main_label_5
	XOR x11 x11 x11
	B calc_label_1
calc_label_1
	XOR x12 x12 x12
	MULi x14 x11 400
	ADDi x15 x11 399
	MODi x15 x15 400
	MULi x15 x15 400
	ADDi x16 x11 400
	MODi x16 x16 400
	MULi x16 x16 400
	ADDi x17 x11 401
	MODi x17 x17 400
	MULi x17 x17 400
	B calc_label_4
calc_label_2
	B calc_label_5
calc_label_3
	INC_EQ x13 x11 400
	BR_COND x13 calc_label_2 calc_label_1
calc_label_4
	XOR x20 x20 x20
	ADD x18 x12 x14
	LOAD x19 x3 x18
	ICMP_EQ x20 x19 1
	SEXT_FROMBOOL x37 x20
	ADDi x22 x12 399
	MODi x22 x22 400
	ADD x23 x15 x22
	LOAD x23 x3 x23
	ADD x37 x23 x37
	ADDi x24 x12 400
	MODi x24 x24 400
	ADD x25 x15 x24
	LOAD x25 x3 x25
	ADD x37 x25 x37
	ADDi x26 x12 401
	MODi x26 x26 400
	ADD x27 x15 x26
	LOAD x27 x3 x27
	ADD x37 x27 x37
	ADD x28 x16 x22
	LOAD x28 x3 x28
	ADD x37 x28 x37
	ADD x29 x16 x24
	LOAD x29 x3 x29
	ADD x37 x29 x37
	ADD x30 x16 x26
	LOAD x30 x3 x30
	ADD x37 x30 x37
	ADD x31 x17 x22
	LOAD x31 x3 x31
	ADD x37 x31 x37
	ADD x32 x17 x24
	LOAD x32 x3 x32
	ADD x37 x32 x37
	ADD x33 x17 x26
	LOAD x33 x3 x33
	ADD x37 x33 x37
	ICMP_EQ x34 x37 3
	ICMP_EQ x35 x37 2
	SELECT_FALSE x35 x35 x20
	OR x35 x35 x34
	STORE x6 x18 x35
	INC_EQ x13 x12 400
	BR_COND x13 calc_label_3 calc_label_4
calc_label_5
	B draw_label_7
draw_label_7
	XOR x11 x11 x11
	B draw_label_8
draw_label_8
	XOR x12 x12 x12
	MULi x14 x11 400
	B draw_label_11
draw_label_9
	B draw_label_12
draw_label_10
	INC_EQ x13 x11 400
	BR_COND x13 draw_label_9 draw_label_8
draw_label_11
	ADD x15 x14 x12
	LOAD x16 x6 x15
	SET_PIXEL x12 x11 x16
	INC_EQ x13 x12 400
	BR_COND x13 draw_label_10 draw_label_11
draw_label_12
	DISPLAY
	SWAP x3 x6
	B main_label_5
