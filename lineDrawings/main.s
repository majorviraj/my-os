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

	fbInfoAddr .req r4
	ldr fbInfoAddr, =frameBufferData

	render$:
		fbAddress .req r3
		ldr fbAddress, [fbInfoAddr, #32]

		colour .req r0
		ldr colour, = #6754
		y .req r1
		mov y, #768

		drawRow$:
			x .req r2
			mov x, #1024

			drawCollum$:
				sub x, #1
				strh colour, [fbAddress]
				add fbAddress, #2

				teq x, #0
				bne drawCollum$

			sub y, #1
			teq y, #0
			bne drawRow$

		ldr r0,= #2000000
		bl Micros

	.unreq fbAddress


/*		#RED
		ldr r0,= #63488

		fbAddr .req r3
		ldr fbAddr,[fbInfoAddr,#32]

		colour .req r0
		y .req r1
		mov y,#768
		drawRow1$:
			x .req r2
			mov x,#1024
			drawPixel1$:
				strh colour,[fbAddr]
				add fbAddr,#2
				sub x,#1
				teq x,#0
				bne drawPixel1$

			sub y,#1
			#add colour,#1
			teq y,#0
			bne drawRow1$

		ldr r0,= #2000000
		bl Micros

		#GREEN
		ldr r0,= #2016

		fbAddr .req r3
		ldr fbAddr,[fbInfoAddr,#32]

		colour .req r0
		y .req r1
		mov y,#768
		drawRow2$:
			x .req r2
			mov x,#1024
			drawPixel2$:
				strh colour,[fbAddr]
				add fbAddr,#2
				sub x,#1
				teq x,#0
				bne drawPixel2$

			sub y,#1
			#add colour,#1
			teq y,#0
			bne drawRow2$

		ldr r0,= #2000000
		bl Micros
*/
	ldr r0, =#5
	ldr r1, =#5

	ldr r2, =#300
	ldr r3, =#100
	bl drawLine


	ldr r0,= #2000000
	bl Micros


	ldr r0, =#456
	ldr r1, =#0
	ldr r2, =#676
	ldr r3, =#221

	bl drawLine
	ldr r0,= #2000000
	bl Micros

		#BLACK
		ldr r0,= #0

		fbAddr .req r3
		ldr fbAddr,[fbInfoAddr,#32]

		colour .req r0
		y .req r1
		mov y,#768
		drawRow6$:
			x .req r2
			mov x,#1024
			drawPixel6$:
				strh colour,[fbAddr]
				add fbAddr,#2
				sub x,#1
				teq x,#0
				bne drawPixel6$

			sub y,#1
			#add colour,#1
			teq y,#0
			bne drawRow6$

		ldr r0,= #2000000
		bl Micros
		.unreq fbAddr
		.unreq fbInfoAddr
