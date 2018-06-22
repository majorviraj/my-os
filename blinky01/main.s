.section .init
@ .globl _start
@ _start:

	@ b main
.section .text
kernel_main:

	ldr r0,=0x20200000
	
	mov r1, #1
	lsl r1, #21
	str r1, [r0,#16]

	loop1$:
		mov r1, #1
		lsl r1, #15
		str r1, [r0, #32]

		bl delay500ms$

		mov r1, #1
		lsl r1, #15
		str r1, [r0, #44]

		bl delay500ms$

		b loop1$


delay500ms$:
	ldr r2,=0x3F0000
	delay$:
		sub r2, #1
		cmp r2, #0
		bne delay$
	mov pc, lr
