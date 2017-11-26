/*Used the cambridge tutorial but had to change FSEL1, GPIO Set, GPIO Clear register addresses as Pi1 used
* GPIO 16 for ACT led whereas Pi B+ uses GPIO 47. 
*/


.section .init
.globl _start
_start:
mov r1,#1
lsl r1,#21 			@bit number
str r1,[r0,#16] 	@r0+16 is the register address for FSEL1.

jump$:
mov r1,#1
lsl r1,#15
str r1,[r0,#32]

mov r2,#0x3F0000
wait1$:
sub r2,#1
cmp r2,#0
bne wait1$

mov r1,#1
lsl r1,#15
str r1,[r0,#44]

mov r2,#0x3F0000
wait2$:
sub r2,#1
cmp r2,#0
bne wait2$

b jump$
