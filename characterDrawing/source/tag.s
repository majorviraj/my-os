.section .text
.globl FindTag
FindTag:
tag .req r0
tagList .req r1
tagAddr .req r2

sub tag,#1
cmp tag,#8
movhi tag,#0
movhi pc,lr

ldr tagList,=tag_core
tagReturn$:
add tagAddr,tagList, tag,lsl #2
ldr tagAddr,[tagAddr]

teq tagAddr,#0
movne r0,tagAddr
movne pc,lr

ldr tagAddr,[tagList]
teq tagAddr,#0
movne r0,#0
movne pc,lr

mov tagAddr,#0x100
push {r4}
tagIndex .req r3
oldAddr .req r4
tagLoop$:
ldrh tagIndex,[tagAddr,#4]
subs tagIndex,#1
poplt {r4}
blt tagReturn$

add tagIndex,tagList, tagIndex,lsl #2
ldr oldAddr,[tagIndex]
teq oldAddr,#0
.unreq oldAddr
streq tagAddr,[tagIndex]

ldr tagIndex,[tagAddr]
add tagAddr, tagIndex,lsl #2
b tagLoop$

.unreq tag
.unreq tagList
.unreq tagAddr
.unreq tagIndex