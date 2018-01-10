/* main file for Screen From Scratch*/

.section .init
.globl _start
_start:

b main

.section .text

main:
	mov sp, #0x8000			@Initialise the stack at 0x8000
							@as the rpi bootloader is setup in such a way
	mov r0, #1024			@width
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


	ldr r0, =#0
	ldr r1, =#0
	ldr r2, =#200
	ldr r3, =#300
	bl drawRectangle

loop1$:
	ldr r0, =#512
	ldr r1, =#384
	ldr r2, =#100

	bl drawFilledCircle

	ldr r0,=0xFFFF
	bl setForeColour

	ldr r0, =#10000
	bl delayMicro
	b loop1$
