.section .data
.align 1

foreColour:
    .hword 0xffff

.align 2

graphicsAddress:
    .int 0

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
    movls stepX, #-1


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
