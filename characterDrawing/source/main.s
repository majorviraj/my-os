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

	ldr r0, =#107
	mov r1, #5
	mov r2, #5

	bl drawCharacter


mov r0, #540
mov r1, #440

bl drawPixel
	ldr r0, =#1000000
	bl delayMicro
