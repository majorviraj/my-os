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

	ldr r4, =#0

loop1$:
	ldr r0, =#512
	ldr r1, =#384
	mov r2, r4

	bl drawCircle

	add r0, r1, #29952
	bl setForeColour

	ldr r0, =#10000
	bl delayMicro

	add r4, #1
	teq r4, #400
	bne loop1$

		b noError$

	@ ldr r0, =#1000000
	@ bl delayMicro	


	@ ldr r0, =#107
	@ mov r1, #5
	@ mov r2, #5

	@ bl drawCharacter

	@ ldr r0, =#1000000
	@ bl delayMicro
