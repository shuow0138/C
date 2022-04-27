;;Shuo Wang
;;116879538
;;swang308 
	.data
	.text
.global is_palindrome
is_palindrome:
	
	movw r26, r24		;load the pointer to X
	push r26		;Push r26 to stack
	push r27		;Push r27 to stack
	movw r30, r24		;load the pointer to Z
	call Strlen		;call strlen
	pop r27			;Pop r27
	pop r26			;Pop r27
	movw r18, r24		;load the strlen to r18
	push r24		;Push r24 to stack
	adc r30, r24		;add the strlen to the first element, then
	adc r31, r25		;we have r31:r30 pointing to the last elemet
3:	

	cpi r18, 0		;compare length with 0 
	breq 2f			;if length is 0, jump to 2f
				;If the length is 0 and no difference occurs,
				;then, the string is palindrome.	 
	clr r20			;clear r20
	ld r20, X+		;load current char to r20 
	clr r21			;clear r21
	ld r21, -Z		;load current char(form backward) to r21 
	dec r18			;decrease the strlen by 1
	cp r20, r21		;Compare those two characters
	brne 1f			;if not equal, jump to 1
	jmp 3b			;if equal jump back to 3
	
1:	pop r24			;Pop r24
	ldi r24, 0		;Load r24 to 0 (false)
	clr r25			;clear r25, since the return value is 8 bit
	ret			;return
	
2:	pop r24			;Pop r24
	ldi r24, 1		;Load r24 to 1 (true)
	clr r25			;clear r25, since the return value is 8 bit
	ret			;return
	
.global Strlen
Strlen:
    ;;; uint16_t Strlen(char *arg)

        mov r26, r24		; copy argument to r
        mov r27, r25		;X (r27:26) pointe
        ldi r24, 0		;initialize r25:r24 to zero
        ldi r25, 0		;( the return value)
2:
        clr r18			;clear r18
        ld  r18, X+		; load X with post-increment
        cpi r18, 0		;Compare r18 with 0
        breq 1f 		;if loaded value was zero, branch 1f
        adiw r24, 1   		; increment return value
        jmp 2b  		; jump 2b (label 2, backward)

1:
        ret    ;; return
