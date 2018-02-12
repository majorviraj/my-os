.section .text.startup

.global _start
_start:

    b _reset_
    b undefinedInstruction
    b softwareInterrupt
    b prefetchAbort
    b _reset_
    b _reset_
    b interruptRequest
    b fastInterrupt

    b _inf_loop

_reset_:
    mov r0, #0x8000
    mov r1, #0x0000
    ldmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
    
    mov sp, #0x8000

    b _bss_clear


.globl _bss_clear
_bss_clear:

    push {r0, r1, r2}
    ldr r0, =__bss_start__
    ldr r2, =__bss_end__

    mov r1, #0
clearBSS$:

    str r1, [r0]
    add r0, #1
    cmp r0, r2
    ble clearBSS$

    pop {r0, r1, r2}
    b kernel_main

    b _inf_loop

.globl _enable_interrupts
_enable_interrupts:
        mrs     r0, cpsr
        bic     r0, r0, #0x80
        msr     cpsr_c, r0

        mov     pc, lr

.globl _inf_loop
_inf_loop:
    b _inf_loop
