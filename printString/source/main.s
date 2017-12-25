/* main file for Screen From Scratch*/

.section .init
.globl _start
_start:

b main

.section .data
.align 2

test:
	.ascii "hello"
	.byte '\n'
	.ascii "world"


.section .text

main:
	mov sp, #0x8000			@Initialise the stack at 0x8000
							@as the rpi bootloader is setup in such a way
	ldr r0, =#1024			@width
	mov r1, #768			@height
	mov r2, #16				@bitDepth
	bl frameBufferInit

	teq r0, #0
	bne noError$

	mov r0,#47
	mov r1,#1
	bl SetGpioFunction

	mov r0,#47
	mov r1,#0
	bl SetGpio

	error$:
		b error$

	noError$:

	mov r0,#9
	bl FindTag
	ldr r1,[r0]
	lsl r1,#2
	sub r1,#8
	add r0,#8
	mov r2,#0
	mov r3,#0
	bl printString

	ldr r0,= #5000000
	bl delayMicro


	ldr r0, =test
	mov r1, #11
	mov r2, #0
	mov r3, #500

	bl printString

	loop1$:
		b loop1$

