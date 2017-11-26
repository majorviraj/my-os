.section .data
.align 1
foreColour:
.hword 0xFFFF

.align 2
graphicsAddress:
.int 0

.section .text
.globl SetForeColour
	SetForeColour:
	cmp r0,#0x10000
	movhi pc,lr
	moveq pc,lr
	ldr r1,=foreColour
	strh r0,[r1]
	mov pc,lr

.globl SetGraphicsAddress
SetGraphicsAddress:
	ldr r1,=graphicsAddress
	str r0,[r1]
	mov pc,lr

.globl DrawPixel /*changes nothing, also preserves the value of r0,r1 */
DrawPixel:
	push {r0, r1}
	px .req r0
	py .req r1
	addr .req r2
	ldr addr,=graphicsAddress
	ldr addr, [addr]
	height .req r3
	ldr height, [addr, #4]
	sub height, #1
	cmp py, height
	movhi pc,lr
	.unreq height
	width .req r3
	ldr width, [addr, #0]
	sub width, #1
	cmp px, width
	movhi pc,lr

	ldr addr, [addr, #32] /*now addr contains address of the first pixel of the framebuffer */
	add width, #1
	mla px, py , width, px /*this performs px = py*width + px */
	.unreq py
	.unreq width
	add addr, px, lsl #1 /*px is shiftd because px*2 is the actual address for any pixel because in high colour every pixel uses 2 bytes */
	.unreq px
	colour .req r3
	ldr colour,=foreColour /*store address of memory where colour value is stored */
	ldrh colour, [colour] /*load the actual colour into colour register */
	strh colour, [addr] 
	.unreq colour
	.unreq addr
	pop {r0,r1}
	mov pc,lr


.globl FullScreenToForeColour
FullScreenToForeColour:
	fbAddr .req r4
	
	ldr fbAddr,= graphicsAddress
	ldr fbAddr, [fbAddr]
	ldr fbAddr, [fbAddr,#32]
	
	colour .req r0
	ldr colour,= foreColour
	ldr colour, [colour]
	y .req r1
	mov y, #768
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
		teq y,#0
		bne drawRow2$
	mov pc,lr
	.unreq fbAddr
	.unreq y
	.unreq x
	
	

