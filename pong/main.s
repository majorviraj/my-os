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

.align 1
ballX:
	.hword 512
.align 1
ballY:
	.hword 512
.align 1
speedX:
	.hword 10
.align 1
speedY:
	.hword 10
.align 1
paddle1Y:
	.hword 300
.align 1
paddle2Y:
	.hword 550
.align 1
paddle1Length:
	.hword 80
.align 1
paddle2Length:
	.hword 80
.align 1
radiusOfBall:
	.hword 10
.align 1
MaxYValue:
	.hword 700
.align 1
MinYValue:
	.hword 100
.align 1
paddle1RightmostX:
	.hword 30
.align 1
paddle1LeftmostX:
	.hword 0
.align 1
paddle2RightmostX:
	.hword 1023
.align 1
paddle2LeftmostX:
	.hword 993
.align 1
oldParameters:
	.hword 0 @+0 oldBallX
	.hword 0 @+2 oldBallY
	.hword 0 @+4 oldPaddle1Y
	.hword 0 @+6 oldPaddle2Y
	.hword 0 @+8 oldP1Score
	.hword 0 @+10 oldP2Score
.align 1
p1Score:
	.hword 0
p2Score:
	.hword 0

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

	mov r0, #47
	mov r1, #1
	bl SetGpioFunction

	mov r0, #47
	mov r1, #0
	bl SetGpio

	error$:
		b error$

	noError$:

	@Add splashscreen and initialisation code and loading etc here

	bl UsbInitialise


	ldr r0,=0xFFFF
	bl setForeColour

	bl FullScreenToForeColour
	ldr r0, = #5000000
	bl delayMicro


	
	ldr r0,= #0
	bl setForeColour

	@Draw top and bottom borders
	ldr r4,= MaxYValue
	ldrh r4, [r4]
	ldr r5,= radiusOfBall
	ldrh r5, [r5]
	add r1, r4, r5
	mov r3, r1
	add r3, r3, #5
	ldr r0,= #0
	ldr r2,= #1023
	bl drawRectangle

	ldr r4,= MinYValue
	ldrh r4, [r4]
	ldr r5,= radiusOfBall
	ldrh r5, [r5]
	sub r1, r4, r5
	mov r3, r1
	sub r1, r1, #5
	ldr r0,= #0
	ldr r2,= #1023
	bl drawRectangle

gameLoop$:
	
	bl keyboardInit
	@copy ballX ballY, paddle1Y, paddle2Y into old parameters array
	ldr r0, =oldParameters
	ldr r1, =ballX
	ldrh r1, [r1]
	strh r1, [r0]
	ldr r1, =ballY
	ldrh r1, [r1]
	strh r1, [r0, #2]
	ldr r1, =paddle1Y
	ldrh r1, [r1]
	strh r1, [r0, #4]
	ldr r1, =paddle2Y
	ldrh r1, [r1]
	strh r1, [r0, #6]
	ldr r1, =p1Score
	ldrh r1, [r1]
	strh r1, [r0, #8]
	ldr r1, =p2Score
	ldrh r1, [r1]
	strh r1, [r0, #10]
	
	
	mov r0, #1
	bl checkInputs			@Takes r0 as input, r0=1 is P1 and r0=2 is P2
							@Return into r0, up arraw and into r1, down arrow state

	@Update paddle1Y according to inputs
	cmp r0, #1	@check if P1 up array is pressed
	ldreq r2,=paddle1Y
	ldreqh r3,[r2]
	addeq r3, #25
	streqh r3, [r2]
	@check if paddle1Y exceeds max possible value
	ldr r5, =MaxYValue
	ldrh r5, [r5]
	ldr r4, =paddle1Length
	ldrh r4, [r4]
	add r3, r3, r4
	cmp r3, r5
	ldr r5, =MaxYValue
	ldrh r5, [r5]
	ldr r4, =paddle1Length
	ldrh r4, [r4]
	sub r5, r5, r4	
	strhih r5, [r2]

	cmp r1, #0	@check if P1 down array is pressed
	ldreq r2,=paddle1Y
	ldreqh r3,[r2]
	subeq r3, #25
	streqh r3, [r2]
	@check if paddle1Y exceeds min possible value
	ldr r5, =MinYValue
	ldrh r5, [r5]
	cmp r3, r5	
	strlsh r5, [r2]


	@Update paddle2Y according to inputs
	mov r0, #2
	bl checkInputs

	cmp r0, #1	@check if P2 up array is pressed
	ldreq r2,=paddle2Y
	ldreqh r3,[r2]
	addeq r3, #25
	streqh r3, [r2]
	@check if paddle2Y exceeds max possible value
	ldr r5, =MaxYValue
	ldrh r5, [r5]
	ldr r4, =paddle2Length
	ldrh r4, [r4]
	add r3, r3, r4
	cmp r3, r5
	ldr r5, =MaxYValue
	ldrh r5, [r5]
	ldr r4, =paddle2Length
	ldrh r4, [r4]
	sub r5, r5, r4	
	strhih r5, [r2]


	cmp r1, #0	@check if P2 down array is pressed
	ldreq r2,=paddle2Y
	ldreqh r3,[r2]
	subeq r3, #25
	streqh r3,[r2]
	@check if paddle2Y exceeds min possible value
	ldr r5, =MinYValue
	ldrh r5, [r5]
	cmp r3, r5	
	strlsh r5, [r2]

	

	@Update coordinates of ball by speedX and speedY
	ldr r2,=ballX
	ldrh r4, [r2]
	ldr r3,=speedX
	ldrh r3, [r3]
	add r5, r4, r3
	strh r5, [r2]


	ldr r2,=ballY
	ldrh r3, [r2]
	ldr r4,=speedY
	ldrh r4, [r4]
	add r5, r3, r4
	strh r5, [r2]

	@Limit ballX and ballY within limits here
	ldr r2,=ballY
	ldrh r3, [r2]
	ldr r4,= MinYValue
	ldrh r4, [r4]
	ldr r5,= MaxYValue
	ldrh r5, [r5]
	cmp r3, r4
	strleh r4, [r2]
	cmp r3, r5
	strgeh r5, [r2]	

	ldr r2,=ballX
	ldrh r3, [r2]
	ldr r4,=#0
	ldr r5,=#1024 
	cmp r3, r4
	strleh r4, [r2]
	cmp r3, r5
	strgeh r5, [r2]


	@Change speedY according to ballY
	
	/*if (bally>maxY | ballY<minY) {
		negate speedy
	}
	*/
	ldr r2,=ballY
	ldrh r3, [r2]
	ldr r4,=speedY
	ldrh r5, [r4]
	ldr r6,=MinYValue
	ldrh r6, [r6]
	ldr r7,=MaxYValue
	ldrh r7, [r7]
	cmp r3, r6
	ble negate$
	cmpge r3, r7
	bge negate$

	b skipChangingYSpeed$

	negate$:
	neg r5, r5
	strh r5, [r4]

skipChangingYSpeed$:

	@Change speedX according to ballY, paddle1Y & paddle2Y only if ballX = paddle1X or paddle2X
	ldr r3,=ballX
	ldr r4,=ballY
	ldr r5,=speedX
	ldr r6,=speedY
	ldr r7,=paddle1Y
	ldr r8,=paddle2Y
	ldr r9,=radiusOfBall
	ldrh r9, [r9]
	ldr r10,=paddle1Length
	ldrh r10, [r10]
	ldr r11,=paddle2Length
	ldrh r11, [r11]

	@Check if ballX is away from paddl1X and paddle2X
	ldrh r0, [r3]
	sub r1, r0, r9	@done to evaluate leftmost ball coordinate

 	ldr r2,= paddle1RightmostX
 	ldrh r2, [r2]
	cmp r1, r2
	ble checkForPaddle1$
	add r1, r0, r9
	ldr r2,= paddle2LeftmostX
	ldrh r2, [r2]
	cmp r1, r2
	bge checkForPaddle2$

	b ballNotNearPaddles$



	checkForPaddle1$:
		ldrh r0, [r4]	@ball is at paddle1, do whatever necessary below
		ldrh r1, [r7]	@paddle1Y
		add r2, r1, r10	@paddle1Y + paddle1Length
		cmp r0, r1
		blt ballNotOnPaddle1$
		cmp r0, r2
		bgt ballNotOnPaddle1$
		ldrh r2, [r5]	@negate speedX as ball as hit paddle
		neg r2, r2
		strh r2, [r5]
		b ballNotNearPaddles$

		ballNotOnPaddle1$:
		ldr r1,= p2Score
		ldrh r2, [r1]
		add r2, #1
		strh r2, [r1]
		@reset ballX ballY to center
		ldr r0, = #500000
		bl delayMicro
		ldr r1,= #512 @Needs to be changed, hardcoded currently
		ldr r2,= #400 @Needs to be changed, hardcoded currently
		strh r1, [r3]
		strh r2, [r4]
		ldrh r2, [r5] @negate speedX, to change intitial ball direction
		neg r2, r2
		strh r2, [r5]
		b ballNotNearPaddles$


	@check if ballX is near paddle2X
	checkForPaddle2$:
		ldrh r0, [r4]	@ballY @load ballY @ball is at paddle2, do whatever necessary below
		ldrh r1, [r8]	@paddle2Y
		add r2, r1, r11 @paddle2Y+ paddle2Length
		cmp r0, r1
		blt ballNotOnPaddle2$
		cmp r0, r2
		bgt ballNotOnPaddle2$
		ldrh r2, [r5] @negate speedX as ball as hit paddle
		neg r2, r2
		strh r2, [r5]
		b ballNotNearPaddles$
		
		ballNotOnPaddle2$:
			ldr r1,= p1Score
			ldrh r2, [r1]
			add r2, #1
			strh r2, [r1]
			@reset ballX ballY to center
			ldr r0, = #500000
			bl delayMicro
			ldr r1,= #512 @Needs to be changed, hardcoded currently
			ldr r2,= #400 @Needs to be changed, hardcoded currently
			strh r1, [r3]
			strh r2, [r4]
			ldrh r2, [r5] @negate speedX, to change intitial ball direction
			neg r2, r2
			strh r2, [r5]

	
	ballNotNearPaddles$:


	
	@Render all the graphics, paddles, ball and p1Score,p2Score

	ldr r0,=#0
	bl setForeColour

	@Draw left paddle1 here
	ldr r0,= paddle1LeftmostX
	ldrh r0, [r0]
	ldr r1,= paddle1Y
	ldrh r1, [r1]
	ldr r4,=paddle1Length
	ldrh r4, [r4]
	ldr r2,= paddle1RightmostX
	ldrh r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	@Draw right paddle2 here
	ldr r0,= paddle2LeftmostX
	ldrh r0, [r0]
	ldr r1,= paddle2Y
	ldrh r1, [r1]
	ldr r4,=paddle2Length
	ldrh r4, [r4]
	ldr r2,= paddle2RightmostX
	ldrh r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	@draw circle
	ldr r0,=ballX
	ldrh r0, [r0]
	ldr r1,=ballY
	ldrh r1, [r1]
	ldr r2,=radiusOfBall
	ldrh r2, [r2]
	bl drawFilledCircle


	ldr r0,= p1Score
	ldrh r0, [r0]
	add r0, r0, #0x30
	ldr r1, =#10
	ldr r2, =#15
	bl drawCharacter


	ldr r0,= p2Score
	ldrh r0, [r0]
	add r0, r0, #0x30
	ldr r1, =#1003
	mov r2, #15
	bl drawCharacter
	

	@Erase previous ball and paddle traces!
	
	ldr r0,= 0xFFFF
	bl setForeColour

	@Draw left paddle1 here
	ldr r0,= paddle1LeftmostX
	ldrh r0, [r0]
	ldr r1,= oldParameters
	ldrh r1, [r1, #4]
	ldr r5, =paddle1Y
	ldrh r5, [r5]
	teq r1, r5
	beq skipErasingPaddle1$
	ldr r4,=paddle1Length
	ldrh r4, [r4]
	ldr r2,= paddle1RightmostX
	ldrh r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	skipErasingPaddle1$:
	@Draw right paddle2 here
	ldr r0,= paddle2LeftmostX
	ldrh r0, [r0]
	ldr r1,= oldParameters
	ldrh r1, [r1, #6]
	ldr r5, =paddle2Y
	ldrh r5, [r5]
	teq r1, r5
	beq skipErasingPaddle2$
	ldr r4,=paddle2Length
	ldrh r4, [r4]
	ldr r2,= paddle2RightmostX
	ldrh r2, [r2]
	add r3, r1, r4
	bl drawRectangle

	skipErasingPaddle2$:
	@draw circle
	ldr r0,=oldParameters
	ldrh r0, [r0]
	ldr r1,=oldParameters
	ldrh r1, [r1, #2]
	ldr r5, =ballX
	ldrh r5, [r5]
	ldr r6, =ballY
	ldrh r6, [r6]
	teq r0, r5
	bne dontSkipErasingBall$
	teq r1, r6
	beq skipErasingBall$
	
	dontSkipErasingBall$:
	
		ldr r2,=radiusOfBall
		ldrh r2, [r2]
		bl drawFilledCircle

	
	skipErasingBall$:
	
	@erase scores
	ldr r0,= p1Score
	ldrh r0, [r0]
	ldr r3, =oldParameters
	ldrh r3, [r3, #8]
	teq r0, r3
	beq skipErasingP1Score$
	add r0, r0, #0x30
	ldr r1,= #10
	ldr r2,= #15
	bl drawCharacter

	skipErasingP1Score$:
	
	ldr r0,= p2Score
	ldrh r0, [r0]
	ldr r3, =oldParameters
	ldrh r3, [r3, #10]
	teq r0, r3
	beq skipErasingP2Score$
	add r0, r0, #0x30
	ldr r1,=#1003
	ldr r2, =#15
	bl drawCharacter
	
	skipErasingP2Score$:
	

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

	ldr r0,=keyboardAddress
	teq r0, #0
	popeq {r4,r5,r6,pc}

	ldr r1, [r0]

	mov r4,r1
	mov r5, #0

	keyLoop$:
		
		ldr r0, = keyboardOldDown
		add r0, r0, r5
		ldrh r0, [r0]

		teq r0, #0x1a
		moveq r0, #0
		moveq r1, #0
		popeq {r4,r5,r6,pc}

		teq r0, #0x16
		moveq r0, #1
		moveq r1, #1
		popeq {r4,r5,r6,pc}

		add r5,#1
		cmp r5,#3
		blt keyLoop$

		mov r0, #2
		mov r1, #2
		pop {r4,r5,r6,pc}

player2loop$:

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
		add r0, r0, r5
		ldrh r0, [r0]

		teq r0, #0x52
		moveq r0, #0
		moveq r1, #0
		popeq {r4,r5,r6,pc}

		teq r0, #0x51
		moveq r0, #1
		moveq r1, #1
		popeq {r4,r5,r6,pc}

		add r5,#1
		cmp r5,#3
		blt keyLoop2$

		mov r0, #2
		mov r1, #2
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
