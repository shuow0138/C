	.file	"isqrt_reference.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	isqrt
	.type	isqrt, @function
isqrt:
	push r28
	push r29
	rcall .
	push __zero_reg__
	in r28,__SP_L__
	in r29,__SP_H__
/* prologue: function */
/* frame size = 3 */
/* stack size = 5 */
.L__stack_usage = 5
	std Y+3,r24
	ldd r24,Y+3
	cpi r24,lo8(2)
	brsh .L2
	ldd r24,Y+3
	rjmp .L3
.L2:
	ldd r24,Y+3
	lsr r24
	lsr r24
	rcall isqrt
	lsl r24
	std Y+1,r24
	ldd r24,Y+1
	subi r24,lo8(-(1))
	std Y+2,r24
	ldd r24,Y+2
	mov r24,r24
	ldi r25,0
	ldd r18,Y+2
	mov r18,r18
	ldi r19,0
	mov r22,r18
	mov r23,r19
	rcall __mulhi3
	mov r18,r24
	mov r19,r25
	ldd r24,Y+3
	mov r24,r24
	ldi r25,0
	cp r24,r18
	cpc r25,r19
	brge .L4
	ldd r24,Y+1
	rjmp .L3
.L4:
	ldd r24,Y+2
.L3:
/* epilogue start */
	pop __tmp_reg__
	pop __tmp_reg__
	pop __tmp_reg__
	pop r29
	pop r28
	ret
	.size	isqrt, .-isqrt
	.ident	"GCC: (Fedora 4.9.2-1.el7) 4.9.2"
