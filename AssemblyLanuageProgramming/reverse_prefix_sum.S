;;Shuo Wang 
;;116879538
;;swang308 
	.data
pctd1:
	.asciz "%d"		;pctd1 is the format string
				;without heading space
pctd2:
	.asciz " %d"		;pctd2 is the format string
				;with a heading space
	.text
.global reverse_prefix_sum
reverse_prefix_sum:
	
	movw r26, r24		;We load the address to X(r27:r26)

        ld  r18, X		;Load the current element to r18
        cpi r18, -1		;Compare r18 with -1
        breq 1f			;If r18 equals to -1, we jump to 1
	adiw r26, 1		;Increase the index by 1
				;(move to next element) 
	
	movw r24, r26		;copy next element's address to r25:r24
				;for resursion (reverse_prefix_sum)
				;have address to the next element
	
	push r18		;Save values of current element
	push r19		;for later addition			
	call reverse_prefix_sum	;Recursion
	pop r19			;Get values of current element 
	pop r18			;for later addition
				
	add r18, r24		;We add the result ofresursion (
	adc r19, r25		;reverse_prefix_sum) to current element.
				
	sbiw r26, 1		;We move to previous element,
				;since we add from back to the beginning
	st X, r18		;Change the value in the array
	movw r24, r18		;copy values in r18 to r24,
				;so that we can return the current value.
	ret			;return value
		
1:	ldi r25, hi8(0)		;If the current value is -1,
	ldi r24, lo8(0)		;we load r25:r24 with 0
	ret			;return 0
	
.global print_array
print_array:
	
        movw r26, r24		;We load the address to X(r27:r26)
        ld  r18, X		;Load the current element to r18
        cpi r18, -1		;Compare r18 with -1
        breq 1f			;If r18 equals to -1, we jump to 1
	adiw r26, 1		;Increase the index by 1
				;(move to next element) 
	mov r24, r18		;copy current element to r25:r24
				;for later print function
	
	push r26	       	;Push r27:r26 to stack
	push r27		;Save values of current element address
	call pint_without_space	;print the first element without heading space
	pop r27			;Pop r27:r26
	pop r26			;Get values of current element address	
	
2:
	ld r18, X		;Load the current element to r18
	cpi r18, -1		;Compare r18 with -1
	breq 1f			;If r18 equals to -1, we jump to 1
	adiw r26, 1		;Increase the index by 1
				;(move to next element)
	mov r24, r18		;copy current element to r25:r24
				;for later print function
	push r26		;Save values of current element address
	push r27		;Push r27:r26 to stack
	call pint_with_space	;print the current element with a heading space
	pop r27			;Pop r27:r26
	pop r26			;Get values of current element address
           
        jmp 2b  		;Since it is not -1, we go back to read next one

1:	
	call prt_newline	;print a new line
        ret   			;return 

pint_without_space:
        ;; prints an integer value, r22/r23 have the format string
        ldi r22, lo8(pctd1)     ; Lower byte of the string address
        ldi r23, hi8(pctd1)     ; Higher byte of the string address
	clr r25			;Since it is 8-bit, we clear r25
        push r25		
        push r24	     	;r25:r24 have the element.
        push r23	     	;r22/r23 have the format string
        push r22		;We push the format string and the value
        call printf		;Print the elements
        pop r22			;Get format string back.
        pop r23			;Pop r23:r22
        pop r24			;Get element back.
        pop r25			;Pop r25:r24

        ret
	
pint_with_space:
        ;; prints an integer value, r23:r22 have the format string
        ldi r22, lo8(pctd2)     ; Lower byte of the string address
        ldi r23, hi8(pctd2)     ; Higher byte of the string address
	clr r25			;Since it is 8-bit, we clear r25
	push r25
        push r24	     	;r25:r24 have the element.
        push r23	     	;r22/r23 have the format string
        push r22		;We push the format string and the value
        call printf		;Print the elements
        pop r22			;Get format string back.
        pop r23			;Pop r23:r22
        pop r24			;Get element back.
        pop r25			;Pop r25:r24
        ret			;return
	
prt_newline:
        clr r25                	; To use putchar we place
        ldi r24, 10       	; the value to print in r25:r24
				; 10 is for '\n'
        call putchar		;print char '\n'
        ret			;return 
