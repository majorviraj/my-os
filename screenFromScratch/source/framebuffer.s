

.section .data

.align 12 	/* So that data can be sent to the GPU in its requred format of lower 4 bits zero*/

.globl frameBufferData
frameBufferData:
.int 1024 	/* #0 Physical Width */
.int 768 	/* #4 Physical Height */
.int 1024 	/* #8 Virtual Width */
.int 768 	/* #12 Virtual Height */
.int 0 		/* #16 GPU - Pitch */
.int 16 	/* #20 Bit Depth */
.int 0 		/* #24 X */
.int 0 		/* #28 Y */
.int 0 		/* #32 GPU - Pointer to the frame buffer in memory*/
.int 0 		/* #36 GPU - Size of the resulting frame buffer*/


.section .text

.global frameBufferInit
frameBufferInit:

push {lr}

width 		.req r0
height 		.req r1
bitDepth 	.req r2

cmp 	width,  #4096
cmpls	height, #4096
cmpls	bitDepth, #32

movhi r0, #0
movhi pc, lr

fbAddress .req r3

ldr fbAddress, =frameBufferData
str width, 		[fbAddress, #0]
str height, 	[fbAddress, #4]
str width, 		[fbAddress, #8]
str height, 	[fbAddress, #12]
str bitDepth, 	[fbAddress, #20]

.unreq width
.unreq height
.unreq bitDepth

mov r0, fbAddress
add r0, #0x40000000
mov r1, #1

push {fbAddress}
bl mailboxWrite

mov r0, #1
bl mailboxRead

pop {fbAddress}

teq r0, #0
movne r0, #0
popne {pc}

mov r0, fbAddress
pop {pc}

.unreq fbAddress
