

.section .init
.globl _start
_start:

b main

.section .data
.align 2

test:
	.byte 'V'
	.byte 'i'
	.byte 'r'
	.byte '\t'
	.byte 't'
	.byte 'a'
	.byte 'n'


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

	@ mov r0,#9
	@ bl FindTag
	@ ldr r1,[r0]
	@ lsl r1,#2
	@ sub r1,#8
	@ add r0,#8
	@ mov r2,#0
	@ mov r3,#0
	@ bl printString
	@ 	loop$:
	@ 	b loop$

	bl UsbInitialise
mov r4, #0
mov r5, #0
loopContinue$:
	bl keyboardInit

	bl KeyboardGetChar

	teq r0, #0
	beq loopContinue$

	mov r1, r4
	mov r2, r5

	bl drawCharacter

	add r4,r0

	teq r4,#1024
	addeq r5,r1
	moveq r4,#0
	teqeq r5,#768
	moveq r5,#0

	b loopContinue$


/*
	ldr r0, =test
	mov r1, #3
	mov r2, #0
	mov r3, #0

	bl printString

	loop$:
		b loop$

/*
ldr r0, =#1023
mov r1, #0
bl drawPixel
loop$:
	b loop$	
*/
