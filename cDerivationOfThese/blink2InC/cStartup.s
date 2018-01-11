.section .text.startup

.global _start
_start:

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
    add r1, #1
    cmp r0, r2
    ble clearBSS$

    pop {r0, r1, r2}
    b kernel_main

    b _inf_loop


.globl _inf_loop
_inf_loop:
    b _inf_loop