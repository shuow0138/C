;;Shuo Wang
;;116879538
;;swang308 
	.data			
	.text
.global isqrt
isqrt:
	cpi r24, 2		;compare with 2
	brlo 1f			;if r24 < 2, jump to 1
	movw r18, r24		; we save n in r18
	push r19		;Push r19 to stack
	push r18		;Push r18 to stack
	lsr r24			;right shift r24
	lsr r24			;right shift r24
	call isqrt		;call resursive function
	pop r18			;Pop r18
	pop r19			;Pop r18
	lsl r24			;Left shift r24
	movw r20, r24		;save small in r20
	
	movw r22, r20		;save large in r22
	inc r22			;increase r22 by 1
				;since large is 1 larger than small
	mul r22, r22		;multiply large
	cp r0, r18		;compare the result stored in r1:r0 to r18
	brlt 2f			;if it is less than
	breq 2f			;or if it is equal to
				;we jump to 2
	
	movw r24, r20		;if not, we copy large to r24
	clr r25			;since the return value should be 8 bit
	jmp 1f			;jump to 1
	
2:	movw r24, r22		;we copy small to r24
	clr r25			;since the return value should be 8 bit
	jmp 1f			;jump to 1
	
1:	ret			;return
