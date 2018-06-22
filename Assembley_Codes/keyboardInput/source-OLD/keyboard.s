.section .data
keyboardAddress:
    .int 0

keyboardOldDown:
    .rept 6
    .hword 0
    .endr

.align 3

KeysNormal:

.byte 0x0, 0x0, 0x0, 0x0, 'a', 'b', 'c', 'd'
.byte 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l'
.byte 'm', 'n', 'o', 'p', 'q', 'r', 's', 't'
.byte 'u', 'v', 'w', 'x', 'y', 'z', '1', '2'
.byte '3', '4', '5', '6', '7', '8', '9', '0'
.byte '\n', 0x0, '\b', '\t', ' ', '-', '=', '['
.byte ']', '\\', '#', ';', '\'', '`', ',', '.'
.byte '/', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+'
.byte '\n', '1', '2', '3', '4', '5', '6', '7'
.byte '8', '9', '0', '.', '\\', 0x0, 0x0, '='


.align 3
KeysShift:

.byte 0x0, 0x0, 0x0, 0x0, 'A', 'B', 'C', 'D'
.byte 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'
.byte 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'
.byte 'U', 'V', 'W', 'X', 'Y', 'Z', '!', '@'
.byte '#', '$', '%', '^', '&', '*', '(', ')'
.byte '\n', 0x0, '\b', '\t', ' ', '_', '+', '{'
.byte '}', '|', '~', ':', '"', '~', '<', '>'
.byte '?', 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
.byte 0x0, 0x0, 0x0, 0x0, '/', '*', '-', '+'
.byte '\n', '1', '2', '3', '4', '5', '6', '7'
.byte '8', '9', '0', '.', '|', 0x0, 0x0, '='

.section .text

.global keyboardInit
keyboardInit:
    
    push {r4,r5,lr}

    @ bl UsbInitialise
    @ teq r0, #0
    @ popne {r4,r5,pc}

    keyboardAdr .req r4
    ldr keyboardAdr, =keyboardAddress
    ldr r0, [keyboardAdr]
    teq r0, #0
    bne haveKeyboards$

    noKeyboardFound$:
        bl UsbCheckForChange

        bl KeyboardCount

        teq r0, #0
        streq r0, [keyboardAdr]
        popeq {r4,r5,pc}

        mov r0,#0
        bl KeyboardGetAddress
        
        teq r0, #0
        popeq {r4,r5,pc}

        str r0, [keyboardAdr]
haveKeyboards$:
        mov r5, #0

        loadKeys$:
            ldr r0, [keyboardAdr]
            mov r1, r5

            bl KeyboardGetKeyDown

            ldr r1, =keyboardOldDown
            add r1, r5, lsl #1

            strh r0, [r1]

            add r5, #1
            cmp r5, #6

            blt loadKeys$
        
        ldr r0, [keyboardAdr]
        bl KeyboardPoll

        teq r0, #0
        bne noKeyboardFound$

        pop {r4, r5, pc}
    	.unreq keyboardAdr

.globl KeyWasDown
KeyWasDown:
	ldr r1,=keyboardOldDown
	mov r2,#0

	keySearch$:
		ldrh r3,[r1]
		teq r3,r0
		moveq r0,#1
		moveq pc,lr

		add r1,#2
		add r2,#1
		cmp r2,#6
		blt keySearch$

	mov r0,#0
	mov pc,lr

.globl KeyboardGetChar
KeyboardGetChar:	
	ldr r0,=keyboardAddress
	ldr r1,[r0]
	teq r1,#0
	moveq r0,#0
	moveq pc,lr

	push {r4,r5,r6,lr}
	
	kbd .req r4
	key .req r6

	mov r4,r1
	mov r5,#0

	keyLoop$:
		mov r0,kbd
		mov r1,r5
		bl KeyboardGetKeyDown

		teq r0,#0
		beq keyLoopBreak$
		
		mov key,r0
		bl KeyWasDown
		teq r0,#0
		bne keyLoopContinue$

		cmp key,#104
		bge keyLoopContinue$

		mov r0,kbd
		bl KeyboardGetModifiers

		tst r0,#0b00100010
		ldreq r0,=KeysNormal
		ldrne r0,=KeysShift

		ldrb r0,[r0,key]
		teq r0,#0
		bne keyboardGetCharReturn$

	keyLoopContinue$:
		add r5,#1
		cmp r5,#6
		blt keyLoop$
		
	keyLoopBreak$:
	mov r0,#0		
keyboardGetCharReturn$:
	pop {r4,r5,r6,pc}
	.unreq kbd
	.unreq key
	