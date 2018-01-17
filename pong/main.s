/*[11/01, 21:44] majorviraj: -Make function to render filled circle but just store in memory
-Make function to transfer bitmap from memory to framebuffer memory area(like char draw)
*Variables made- ballX, ballY, speedX, speedY, paddle1Y, paddle2Y,  p1Score, p2Score,
-Make splash screen


[11/01, 21:44] majorviraj: In main loop -
*check P1 & P2 keys and update paddle1Y & 2Y- make function to check keyboard inputs
*update ballX & ballY according to speedX & speedY
*check ballX & ballY and update speedX, speedY
*compare ballY, paddle1Y & 2Y only when ballX = paddle1X or paddle2X and accordingly update p1Score or p2Score, Inc speed and reset ballX,ballY to center and speedX, speedY to random number(refer bakinPi)
*Render all necessary graphics
*/

.section .data
circleBitmap:
	.rept 200
	.byte 0
	.endr
.align 2
ballX:
	.hword 512
.align 2
ballY:
	.hword 512
.align 2
speedX:
	.hword 10
.align 2
speedY:
	.hword 10
.align 2
paddle1Y:
	.hword 40
.align 2
paddle2Y:
	.hword 40
.align 2
paddle1Length:
	.hword 80
.align 2
paddle2Length:
	.hword 80
.align 2
radiusOfBall:
	.hword 20
.align 2
MaxYValue:
	.hword 700
.align 2
MinYValue:
	.hword 100
.align 2
paddle1RightmostX:
	.hword 180
.align 2
paddle1LeftmostX:
	.hword 150
.align 2
paddle2RightmostX:
	.hword 1000
.align 2
paddle2LeftmostX:
	.hword 970

p1Score:
	.byte 0
p2Score:
	.byte 0

.section .init
.globl _start
_start:
	b main

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

	@Add splashscreen and initialisation code and loading etc here

	bl UsbInitialise


gameLoop$:
	mov r0, #1
	bl checkInputs			@Takes r0 as input, r0=1 is P1 and r0=2 is P2
							@Return into r0, up arraw and into r1, down arrow state

	@Update paddle1Y according to inputs
	cmp r0, #0	@check if P1 up array is pressed
	ldreq r2,=paddle1Y
	ldreq r3,[r2]
	addeq r3, #1
	streq r3, [r2]
	@check if paddle1Y exceeds max possible value
	ldr r5, =MaxYValue
	ldr r5, [r5]
	cmp r3, r5
	movhi r4, r5
	strhi r4, [r2]

	cmp r1, #1	@check if P1 down array is pressed
	ldreq r2,=paddle1Y
	ldreq r3,[r2]
	subeq r3, #1
	streq r3, [r2]
	@check if paddle1Y exceeds min possible value
	ldr r5, =MinYValue
	ldr r5, [r5]
	cmp r3, r5
	movls r4, r5
	strls r4, [r2]


	@Update paddle2Y according to inputs
	mov r0, #2
	bl checkInputs

	cmp r0, #0	@check if P2 up array is pressed
	ldreq r2,=paddle2Y
	ldreq r3,[r2]
	addeq r3, #1
	streq r3, [r2]
	@check if paddle2Y exceeds max possible value
	ldr r5, =MaxYValue
	ldr r5, [r5]
	cmp r3, r5
	movhi r4, r5
	strhi r4, [r2]

	cmp r1, #1	@check if P2 down array is pressed
	ldreq r2,=paddle2Y
	ldreq r3,[r2]
	subeq r3, #1
	streq r3,[r2]
	@check if paddle2Y exceeds min possible value
	ldr r5, =MinYValue
	ldr r5, [r5]
	cmp r3, r5
	movls r4, r5
	strls r4, [r2]


	@Update coordinates of ball by speedX and speedY
	ldr r2,=ballX
	ldr r4, [r2]
	ldr r3,=speedX
	ldr r3, [r3]
	add r5, r4, r3
	str r5, [r2]

	ldr r2,=ballY
	ldr r4, [r2]
	ldr r3,=speedY
	ldr r3, [r3]
	add r5, r4, r3
	str r5, [r2]

	@Limit ballX and ballY within limits here





	@Change speedY according to ballY
	ldr r2,=ballY
	ldr r3, [r2]
	ldr r4,=speedY
	ldr r5, [r4]
	ldr r6, =MaxYValue
	ldr r6, [r6]
	cmp r3, r6
	ldr r6, =MaxYValue
	ldr r6, [r6]
	cmple r3, r6
	bge skipChangingYSpeed$
	neg r5,r5
	str r5, [r4]
	skipChangingYSpeed$:

	@Change speedX according to ballY, paddle1Y & paddle2Y only if ballX = paddle1X or paddle2X
	ldr r3,=ballX
	ldr r4,=ballY
	ldr r5,=speedX
	ldr r6,=speedY
	ldr r7,=paddle1Y
	ldr r8,=paddle2Y
	ldr r9,=radiusOfBall
	ldr r9, [r9]
	ldr r10,=paddle1Length
	ldr r10, [r10]
	ldr r11,=paddle2Length
	ldr r11, [r11]

	@Check if ballX is away from paddl1X and paddle2X
	ldr r0, [r3]
	sub r1, r0, r9	@done to evaluate leftmost ball coordinate

 	ldr r2,= paddle1RightmostX
 	ldr r2, [r2]
	cmp r1, r2
	addne r1, r0, r9
	ldrne r2,= paddle2LeftmostX
	ldrne r2, [r2]
	cmpne r1, r2
	bne ballNotNearPaddles$


		@Check if ballX is near paddle1X
		ldr r0, [r3]
		sub r1, r0, r9 @done to evaluate leftmost ball coordinate
		ldr r2,= paddle1RightmostX
		ldr r2, [r2]
		teq r1, r2
		bne checkForPaddle2$
		ldr r0, [r4]	@ball is at paddle1, do whatever necessary below
		ldr r1, [r7]
		add r2, r1, r10
		cmp r0, r1
		cmpge r0, r2
		ldrle r2, [r5]	@negate speedX as ball as hit paddle
		negle r2, r2
		strle r2, [r5]
		ble checkForPaddle2$
		ldr r1,= p2Score
		ldr r2, [r1]
		add r2, #1
		str r2, [r1]
		@reset ballX ballY to center
		ldr r2,= #512 @Needs to be changed
		str r2, [r3]
		str r2, [r4]


	@check if ballX is near paddle2X
	checkForPaddle2$:
		ldr r0, [r3]	@load ballX
		add r1, r0, r9 @done to evaluate rightmost ball coordinate
		ldr r2,= paddle2LeftmostX
		teq r1, r2
		bne ballNotNearPaddles$
		ldr r0, [r4]	@load ballY @ball is at paddle2, do whatever necessary below
		ldr r1, [r8]
		add r2, r1, r11
		cmp r0, r1
		cmpge r0, r2
		ldrle r2, [r5] @negate speedX as ball as hit paddle
		negle r2, r2
		strle r2, [r5]
		ble ballNotNearPaddles$
		ldr r1,= p1Score
		ldr r2, [r1]
		add r2, #1
		str r2, [r1]
		@reset ballX ballY to center
		ldr r2,= #512 @Needs to be changed, hardcoded currently
		str r2, [r3]
		str r2, [r4]


	ballNotNearPaddles$:

	@Render all the graphics, paddles, ball and p1Score,p2Score

	@Draw left paddle1 here
	ldr r0,= paddle1LeftmostX
	ldr r0, [r0]
	ldr r1,= paddle1Y
	ldr r1, [r1]
	ldr r4,=paddle1Length
	ldr r4, [r4]
	ldr r2,= paddle1RightmostX
	ldr r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	@Draw right paddle2 here
	ldr r0,= paddle2LeftmostX
	ldr r0, [r0]
	ldr r1,= paddle2Y
	ldr r1, [r1]
	ldr r4,=paddle2Length
	ldr r4, [r4]
	ldr r2,= paddle2RightmostX
	ldr r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	@draw circle

	b gameLoop$

.globl checkInputs
checkInputs:

	cmp r0, #3
	movge pc, lr

	push {r4,r5,r6,lr}

	teq r0, #1
	beq player1loop$

	teq r0, #2
	beq player2loop$

player1loop$:

	bl keyboardInit

	ldr r0,=keyboardAddress
	teq r0,#0
	popeq {r4,r5,r6,pc}

	ldr r1,[r0]
	teq r1,#0
	beq player1loop$

	mov r4,r1
	mov r5,#0

	keyLoop$:
		
		ldr r0, =keyboardOldDown
		ldrh r0, [r0]

		teq r0, #0x1a
		moveq r0, #1
		moveq r1, #0
		popeq {r4,r5,r6,pc}

		teq r0, #0x16
		moveq r0, #0
		moveq r1, #1
		popeq {r4,r5,r6,pc}

		add r5,#1
		cmp r5,#6
		blt keyLoop$

		pop {r4,r5,r6,pc}

player2loop$:
	bl keyboardInit

	ldr r0,=keyboardAddress
	teq r0,#0
	beq player1loop$

	ldr r1,[r0]
	teq r1,#0
	beq player1loop$

	mov r4,r1
	mov r5,#0

	keyLoop2$:
		ldr r0, =keyboardOldDown
		ldrh r0, [r0]

		teq r0, #0x52
		moveq r0, #1
		moveq r1, #0
		popeq {r4,r5,r6,pc}

		teq r0, #0x51
		moveq r0, #0
		moveq r1, #1
		popeq {r4,r5,r6,pc}

		add r5,#1
		cmp r5,#6
		blt keyLoop2$

		pop {r4,r5,r6,pc}
/*
.globl renderFilledCircleToMemory
renderFilledCircleToMemory:
push {r4,r5,r6,r7,r8,r9,lr}

	cx .req r4
	cy .req r5
	radius .req r6

	cmp r0, #0
	cmpge r1, #0
	cmpge r2, #0
	poplt {r4,r5,r6,r7,r8,r9,pc}

	mov cx, r0
	mov cy, r1
	mov radius, r2

	x .req r7
	y .req r8
	error .req r9

	mov error, #1
	sub error, radius
	mov x, radius
	sub x, #1
	mov y, #0

	filledCircleRenderLoop$:

		@set pixels 8 times************************************
		@line from x,y to -x,y
		mov r0, x
		mov r1, y
		neg r2, r0
		mov r3, y
		add r0, cx
		add r1, cy
		add r2, cx
		add r3, cy
		bl drawLine

		@line from y,x to -y,x
		mov r0, y
		mov r1, x
		neg r2, y
		mov r3, x
		add r0, cx
		add r1, cy
		add r2, cx
		add r3, cy
		bl drawLine

		@line from x,-y to -x,-y
		mov r0, x
		neg r1, y
		neg r2, x
		neg r3, y
		add r0, cx
		add r1, cy
		add r2, cx
		add r3, cy
		bl drawLine

		@line from y,-x to -y,-x
		mov r0, y
		neg r1, x
		neg r2, y
		neg r3, x
		add r0, cx
		add r1, cy
		add r2, cx
		add r3, cy
		bl drawLine

		cmp error, #0
		add error, y, lsl #1
		add error, #1

		subge error, x, lsl #1
		subge x, #1

		add y, #1
		cmp y, x
		popgt {r4,r5,r6,r7,r8,r9,pc}

		b filledCircleRenderLoop$

	.unreq cx
	.unreq cy
	.unreq radius
	.unreq x
	.unreq y
	.unreq error
*/
