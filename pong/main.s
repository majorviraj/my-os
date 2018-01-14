/*[11/01, 21:44] majorviraj: -Make function to render filled circle but just store in memory
-Make function to transfer bitmap from memory to framebuffer memory area(like char draw)
*Variables made- ballX, ballY, speedX, speedY, paddle1Y, paddle2Y,  p1Score, p2Score, 
-Make splash screen


[11/01, 21:44] majorviraj: In main loop -
*check P1 & P2 keys and update paddle1Y & 2Y- make function to check keyboard inputs
-update ballX & ballY according to speedX & speedY
-compare ballY, paddle1Y & 2Y only when ballX = paddle1X or paddle2X and accordingly update p1Score or p2Score, Inc speed and reset ballX,ballY to center and speedX, speedY to random number(refer bakinPi)
-check ballX & ballY and update speedX, speedY
-update ballX & ballY according to speedX & speedY if ballX and ballY are within limits
*/

.section .data
circleBitmap:
	.byte 0
	.skip 200
.align 2
ballX:
	.hword 0
.align 2
ballY:
	.hword 0
.align 2
speedX:
	.hword 0
.align 2
speedY:
	.hword 0
.align 2
paddle1Y:
	.hword 0
.align 2
paddle2Y:
	.hword 0
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
	
	mov r0, #1
	bl checkInputs			@Takes r0 as inpute, r0=1 is P1 and r0=2 is P2
							@Return in to r0, up arraw and into r1, down arrow state
	@Update paddle1Y according to inputs
	cmp r0, #0	@check if P1 up array is pressed
	ldreq r2,=paddle1Y
	ldreq r3,[r2]
	addeq r3, #1
	streq r3, [r2]
	@check if paddle1Y exceeds max possible value
	cmp r3, SUBSTITUTE Y COORDINATE MAX
	movhi r4, SUBSTITUTE Y COORDINATE MAX
	strhi r4, [r2]
	
	cmp r1, #1	@check if P1 down array is pressed
	ldreq r2,=paddle1Y
	ldreq r3,[r2]
	subeq r3, #1
	streq r3, [r2]
	@check if paddle1Y exceeds max possible value
	cmp r3, SUBSTITUTE Y COORDINATE MIN
	movls r4, SUBSTITUTE Y COORDINATE MIN
	strls r4, [r2]


	@Update paddle12 according to inputs
	mov r0, #2
	bl checkInputs

	cmp r0, #0	@check if P2 up array is pressed
	ldreq r2,=paddle2Y
	ldreq r3,[r2]
	addeq r3, #1
	streq r3, [r2]
	@check if paddle2Y exceeds max possible value
	cmp r3, SUBSTITUTE Y COORDINATE MAX
	movhi r4, SUBSTITUTE Y COORDINATE MAX
	strhi r4, [r2]

	cmp r1, #1	@check if P2 down array is pressed
	ldreq r2,=paddle2Y
	ldreq r3,[r2]
	subeq r3, #1
	streq r3,[r2]
	@check if paddle2Y exceeds max possible value
	cmp r3, SUBSTITUTE Y COORDINATE MIN
	movls r4, SUBSTITUTE Y COORDINATE MIN
	strls r4, [r2]



	

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
