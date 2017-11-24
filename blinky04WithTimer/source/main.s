/*OK03 example from BakePI, modified to work for pin 47 for ACT LED */


.section .init
.globl _start
_start:

b main

.section .text
main:
mov sp,#0x8000
pinNum .req r0
pinFunc .req r1
mov pinNum,#47
mov pinFunc,#1
bl SetGpioFunction
.unreq pinNum
.unreq pinFunc

pinNum .req r0
pinVal .req r1
mov pinNum,#47
mov pinVal,#0
bl SetGpio
.unreq pinNum
.unreq pinVal

/*delay block */
ldr r0,=2000000
bl Micros

pinNum .req r0
pinVal .req r1
mov pinNum,#47
mov pinVal,#1
bl SetGpio
.unreq pinNum
.unreq pinVal

/*delay block */
ldr r0,=8000000
bl Micros

b main