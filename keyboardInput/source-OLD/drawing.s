.section .data
.align 1

foreColour:
    .hword 0xffff

.align 2

graphicsAddress:
    .int 0

.align 4
font:
    .incbin "fontMono.bin"

.section .text
.align 2

.globl setGraphicsAddress
setGraphicsAddress:
    ldr r1, =graphicsAddress
    str r0, [r1]
    mov pc, lr

.globl setForeColour
setForeColour:
    cmp r0, #0x10000
    movhs pc,lr
    ldr r1, =foreColour
    strh r0, [r1]
    mov pc,lr

.globl drawPixel
drawPixel:
    x .req r0
    y .req r1

    ldr r2, =graphicsAddress
    ldr r2, [r2]
    

    ldr r3, [r2, #12]
    cmp y, r3
    movhs pc, lr

    ldr r3, [r2, #8]
    cmp x ,r3
    movhs pc, lr

    width .req r3

    mla r2, y, width, x

    .unreq width
    .unreq x
    .unreq y
    pixelAddress .req r2

    ldr r3, =frameBufferData
    ldr r3, [r3, #32]

    add r3, pixelAddress, lsl #1
    mov pixelAddress, r3

    ldr  r0, =foreColour
    ldrh r0, [r0]

    strh r0, [pixelAddress]

    mov pc, lr

    .unreq pixelAddress

.globl drawLine
drawLine:

    push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}

    x0 .req r9
    y0 .req r10
    x1 .req r11
    y1 .req r12

    mov x0, r0
    mov y0, r1
    mov x1, r2
    mov y1, r3

    deltaX .req r4
    deltaY .req r5
    stepX  .req r6
    stepY  .req r7
    err    .req r8

    cmp x1, x0
    subge deltaX, x1, x0
    movge stepX, #1

    subls deltaX, x0, x1
    movls stepX, #-1

    cmp y1, y0                      
    subge deltaY, y0, y1            @Done as only negative deltaY is used
    movge stepY, #1

    subls deltaY, y1, y0
    movls stepY, #-1


    add err, deltaX, deltaY
    add x1, stepX
    add y1, stepY

    decisionLoop$:
        teq x0, x1
        teqne y0, y1
        popeq {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}

        mov r0, x0
        mov r1, y0

        bl drawPixel                @No pramaters passed as they are already present in r0 and r1 respectively

        cmp deltaY, err, lsl #1
        addle err, deltaY
        addle x0, stepX

        cmp deltaX, err, lsl #1
        addge err, deltaX
        addge y0, stepY

        b decisionLoop$

    .unreq x0
    .unreq y0
    .unreq x1
    .unreq y1

    .unreq deltaY
    .unreq deltaX
    .unreq stepX
    .unreq stepY
    .unreq err

.globl drawCharacter
drawCharacter:
    char .req r4
    x .req r5
    y .req r6

    cmp r0, #127
    movhi r0, #0
    movhi r1, #0
    movhi pc, lr

    push {r4,r5,r6,r7,r8,lr}

    mov char, r0
    mov x, r1
    mov y, r2

    charAddress .req r7
    pixelByte .req r8
    ldr charAddress, =font
    add charAddress, char, lsl #4
    .unreq char

    printChar$:
        ldrb pixelByte, [charAddress]
        counter .req r4
        mov counter, #8
        add x, #8

            loop2$:
                subs counter, #1
                blt endOfLoop$
                
                lsl pixelByte, #1
                tst pixelByte, #0x100
                movne r0, x
                movne r1, y
                blne drawPixel
                
                sub x, #1

                b loop2$
            endOfLoop$:
                add y, #1

                add charAddress, #1
                tst charAddress, #15
                bne printChar$

    mov r0, #8
    mov r1, #16

    pop {r4,r5,r6,r7,r8,lr}

    .unreq x
    .unreq y
    .unreq charAddress
    .unreq counter
    .unreq pixelByte

.globl printString
printString:
    push {r4,r5,r6,r7,r8,r9,lr}
    
    string .req r4
    length .req r5
    x .req r6
    y .req r7
    char .req r8
    orignalX .req r9

    mov string, r0
    mov length, r1
    mov x, r2
    mov orignalX, r2
    mov y, r3

    mov r0, #0x20           //Printing null to get height and width of the characters
    mov r1, #0
    mov r2, #0
    bl drawCharacter

    charWidth .req r0
    charHeight .req r1 

    charPrintLoop$:
        ldrb char, [string]
        add string, #1

        teq char, #0x0a   //0x0a is \n
        addeq y, charHeight
        moveq x, orignalX
        beq charPrintLoop$

        teq char, #0x09   //0x09 is \t
        addeq x, charWidth, lsl #2
        beq charPrintLoop$

        cmp x, #1024
        addeq y, charHeight
        moveq x, orignalX

        mov r0, char
        mov r1, x
        mov r2, y
        bl drawCharacter
        add x, charWidth

        subs length, #1
        bne charPrintLoop$

    pop {r4,r5,r6,r7,r8,r9,lr}

    .unreq string
    .unreq length
    .unreq x
    .unreq y
    .unreq char
    .unreq orignalX
    .unreq charWidth
    .unreq charHeight

.globl drawCircle
drawCircle:
    
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

    drawloop$:

        /*set pixels 8 times*************************************/
        mov r0, x
        mov r1, y
        add r0, cx
        add r1, cy
        bl drawPixel

        mov r0, y
        mov r1, x
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, y
        mov r1, x
        neg r0, r0
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, x
        mov r1, y
        neg r0, r0
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, x
        mov r1, y
        neg r0, r0
        neg r1, r1
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, y
        mov r1, x
        neg r0, r0
        neg r1, r1
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, y
        mov r1, x
        neg r1, r1
        add r0, cx
        add r1, cy
        bl drawPixel
        
        mov r0, x
        mov r1, y
        neg r1, r1
        add r0, cx
        add r1, cy
        bl drawPixel

        cmp error, #0
        add error, y, lsl #1
        add error, #1

        subge error, x, lsl #1
        subge x, #1

        add y, #1
        cmp y, x
        popgt {r4,r5,r6,r7,r8,r9,pc}

        b drawloop$ 

    .unreq cx
    .unreq cy
    .unreq radius
    .unreq x
    .unreq y
    .unreq error


