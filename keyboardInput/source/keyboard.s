.section .data
keyboardAddress:
    .int 0

keyboardOldDown:
    .rept 6
    .hword 0
    .endr

.section .text

.global keyboardInit
keyboardInit:
    
    push {lr}

    bl UsbInitialise
    
    teq r0, #0
    popne {pc}

    keyboardAdr .req r4
    ldr keyboardAdr, =keyboardAddress
    ldr r0, [keyboardAdr]
    teq r0, #0
    bne loc9

    noKeyboardFound$:
        bl UsbCheckForChange

        bl KeyboardCount

        teq r0, #0
        streq r0, [keyboardAdr]
        popeq {pc}

        beq noKeyboardFound$

        mov r0,#0
        bl KeyboardGetAddress
        
        teq r0, #0
        popne {pc}

        str r0, [keyboardAdr]

        












