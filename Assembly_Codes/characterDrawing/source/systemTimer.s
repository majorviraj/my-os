/* micros function generates a delay of r0 microseconds. 2^32 = 4294967 milliseconds = 4294 seconds =  71.582 minutes = 1.193 hours. Clock freq is 1 MHz.*/

.globl GetTimerBase
GetTimerBase:
ldr r0,=0x20003000
mov pc,lr

.globl GetTimeStamp
GetTimeStamp:
push {lr}
bl GetTimerBase
ldrd r0,r1, [r0,#4]
pop {pc}

.globl delayMicro
delayMicro:
delayTime .req r2
mov delayTime, r0

push {lr}

bl GetTimeStamp
start .req r3
mov start, r0
waitLoop$:
bl GetTimeStamp
elapsed .req r1
sub elapsed, r0, start
cmp elapsed, delayTime
.unreq elapsed
bls waitLoop$
.unreq delayTime
.unreq start 
pop {pc}

