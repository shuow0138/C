;;; Shuo Wang
;;; 116879538
;;; swang308

;;; Implementation that computes the nth fibonacci number.
	.data
	
	.text
	
.global fib                	; exposes fib as a label for the linker
fib:                            ; the label: the next instruction starts fib() function.

	cpi r25, 0		;compare higher bytes with 0
	brne 1f			;if not 0, jump to 1
	cpi r24, 0		;compare lower bytes with 0
	brne 1f			;if not 0, jump to 1

	ldi r24, lo8(0)		;if equal to 0,
	ldi r25, hi8(0)		; load r24, r25 with 0
	ret			;return 
	
1:	cpi r25, 0		;compare higher bytes with 0
	brne 2f			;if not 0, jump to 2
	cpi r24, 1		;if high bytes is 0, compare lower bytes w/ 1
	brne 2f			;if not 1, jump to 2

	ldi r24, lo8(1)		;if equal to 1, 
	ldi r25, hi8(1)		;load r24, r25 with 1
	ret			;return 
	
2:	sbiw r24, 1		;r25:r24 minus 1
	push r25		;push r25 to stack
	push r24		;push r24 to stack
	call fib		;call fib
	mov r20, r24		;move results (fib(n-1)) of fib 
	mov r21, r25		;to r21:r20 
	pop r24			;Pop r24
	pop r25			;Pop r25
	
	sbiw r24, 1		;substract r25:r24 by 1
	push r21		;push r21 to stack
	push r20		;push r20 to stack
	call fib		;call fib
	pop r20			;Pop r20
	pop r21			;Pop r21
	
	add r24, r20		;add r21:r20 (fib(n-1))
	adc r25, r21		;to r25:r24 (fib(n-2))
	
	ret                     ; returns.  necessary.
