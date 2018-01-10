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

	mov r0, #47
	mov r1, #0
	bl SetGpio

	error$:
		b error$


	noError$:

	/*Set address of frame buffer in r0 in our GraphicsAddress variable */
	bl SetGraphicsAddress

	/*Set colour to apply to pixels */
	colour .req r0
	ldr colour,= #2016
	bl SetForeColour
	.unreq colour

	bl FullScreenToForeColour

	ldr r0,= #2000000
	bl Micros

	/*black out screen*/
	colour .req r0
	ldr colour,= #0
	bl SetForeColour
	.unreq colour

	bl FullScreenToForeColour
	
	ldr r0,= #2000000
	bl Micros

	#set colour to red
	colour .req r0
	ldr colour,= #63488
	bl SetForeColour
	.unreq colour
	
	@ Routine to test DrawPixel
	x .req r0
	y .req r1
	ldr x,=#400
	ldr y,=#400
	bl DrawPixel
	ldr x,=#401
	ldr y,=#401
	bl DrawPixel
	ldr x,=#402
	ldr y,=#402
	bl DrawPixel
	ldr x,=#403
	ldr y,=#403
	bl DrawPixel
	ldr x,=#404
	ldr y,=#404
	bl DrawPixel
	ldr x,=#405
	ldr y,=#405
	bl DrawPixel
	ldr x,=#406
	ldr y,=#406
	bl DrawPixel
	ldr x,=#407
	ldr y,=#407
	bl DrawPixel
	.unreq x
	.unreq y

	ldr r0,= #5000000
	bl Micros
