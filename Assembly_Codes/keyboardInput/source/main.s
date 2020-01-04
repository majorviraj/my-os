.section .init


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

charToPrint:
	.byte 0


.section .text

.globl kernel_main
kernel_main:
	mov sp, #0x8000			@Initialise the stack at 0x8000
							@as the rpi bootloader is setup in such a way
	ldr r0, =#1920			@width
	ldr r1, =#1080			@height
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

	bl UsbInitialise
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

	mov r4, #0
	mov r5, #0

loopContinue$:
	bl keyboardInit

	bl KeyboardGetChar

	teq r0, #0
	beq loopContinue$

	ldr r6, =charToPrint
	strb r0, [r6]
	mov r0, r6
	mov r1, #1
	mov r2, r4
	mov r3, r5

	bl printString

	add r4,r0

	teq r4,#1024
	addeq r5,r1
	moveq r4,#0
	teqeq r5,#768
	moveq r5,#0

	b loopContinue$
