

.section .init
.globl _start
_start:

/*
* Branch to the actual main code.
*/
b main

/*
* This command tells the assembler to put this code with the rest.
*/
.section .text

main:

/*
* Set the stack point to 0x8000.
*/
	mov sp,#0x8000

/* NEW
* Setup the screen.
*/
	mov r0,#1024
	mov r1,#768
	mov r2,#16
	bl InitialiseFrameBuffer

/* NEW
* Check for a failed frame buffer.
*/

	teq r0,#0
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
	mov fbInfoAddr,r0

/* NEW
* CALL FUNCTION TO WRITE A COLOR IN R0 to COMPLETE
*/

	#RED
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

	#BLUE
	ldr r0,= #31

	fbAddr .req r3
	ldr fbAddr,[fbInfoAddr,#32]

	colour .req r0
	y .req r1
	mov y,#768
	drawRow3$:
		x .req r2
		mov x,#1024
		drawPixel3$:
			strh colour,[fbAddr]
			add fbAddr,#2
			sub x,#1
			teq x,#0
			bne drawPixel3$

		sub y,#1
		#add colour,#1
		teq y,#0
		bne drawRow3$

	ldr r0,= #2000000
	bl Micros


	#WHITE
	ldr r0,= #65535

	fbAddr .req r3
	ldr fbAddr,[fbInfoAddr,#32]

	colour .req r0
	y .req r1
	mov y,#768
	drawRow5$:
		x .req r2
		mov x,#1024
		drawPixel5$:
			strh colour,[fbAddr]
			add fbAddr,#2
			sub x,#1
			teq x,#0
			bne drawPixel5$

		sub y,#1
		#add colour,#1
		teq y,#0
		bne drawRow5$

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
