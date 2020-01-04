/* mailbox */

.globl getMailBoxBaseAddress
getMailBoxBaseAddress:
    ldr r0,=0x2000B880
    mov pc,lr


/*Write data to Mailbox*/
.globl mailboxWrite
mailboxWrite:
    push {lr}
    mov r3, r0

    bl getMailBoxBaseAddress

    channel .req r1
    message .req r3
    status .req r2
    mailbox .req r0

    cmp channel, #15
    pophi {pc}

    wait1$:
    ldr status, [mailbox, #0x18]
    tst status, #0x80000000
    bne wait1$

    add channel, message

    str channel, [mailbox, #0x20]

    .unreq channel
    .unreq message
    .unreq status
    .unreq mailbox

    pop {pc}


/*Methord to read response from mailbox*/
.globl mailboxRead
mailboxRead:
    push {lr}

    cmp r0, #15
    movhi pc, lr

    mov r1, r0

    bl getMailBoxBaseAddress

    channel .req r1
    inchannel .req r3
    message .req r2
    mailbox .req r0

    wait2$:
        status .req r3
        ldr status, [mailbox, #0x18]
        tst status, #0x40000000
        .unreq status
        bne wait2$

    ldr message, [mailbox, #0]

    and inchannel, message, #0b1111
    teq inchannel, channel
    bne wait2$

    and mailbox, message, #0xfffffff0

    .unreq inchannel
    .unreq channel
    .unreq message
    .unreq mailbox

    pop {pc}
