	;
	.data

elems: .byte 10, 20, 7, -1

	.text

.global main
main:
	call init_serial_stdio

	ldi r24, lo8(elems) ; initializing argument
	ldi r25, hi8(elems)

	call print_array

	cli 	; stopping program
	sleep

	ret


