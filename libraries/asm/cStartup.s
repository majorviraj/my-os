.section .text.startup


.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// See ARM section A2.5 (Program status registers)
.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

.equ	SCTLR_ENABLE_DATA_CACHE,        0x4
.equ	SCTLR_ENABLE_BRANCH_PREDICTION, 0x800
.equ	SCTLR_ENABLE_INSTRUCTION_CACHE, 0x1000


.global _start
_start:

	ldr pc, _reset_h
	ldr pc, _undefined_instruction_vector_h
	ldr pc, _software_interrupt_vector_h
	ldr pc, _prefetch_abort_vector_h
	ldr pc, _data_abort_vector_h
	ldr pc, _unused_handler_h
	ldr pc, _interrupt_vector_h
	ldr pc, _fast_interrupt_vector_h

	_reset_h:                           .word   _reset_
	_undefined_instruction_vector_h:    .word   undefinedInstruction
	_software_interrupt_vector_h:       .word   softwareInterrupt
	_prefetch_abort_vector_h:           .word   prefetchAbort
	_data_abort_vector_h:               .word   _reset_
	_unused_handler_h:                  .word   _reset_
	_interrupt_vector_h:                .word   interruptRequest
	_fast_interrupt_vector_h:           .word   fastInterrupt


	b _inf_loop

_reset_:
	@ A simple hack to load the vector table to 0x0000
	mov r0, #0x8000
	mov r1, #0x0000
	ldmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}
	ldmia r0!, {r2,r3,r4,r5,r6,r7,r8,r9}
	stmia r1!, {r2,r3,r4,r5,r6,r7,r8,r9}

	@ Stack pointer for the IRQ mode is being set here
	mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x7000

	@ Stack pointer for the SVR (SUPERVISER MODE) mode is being set here 
	@ (NOTE: SVR mode is the mode that our kernel runs in)
	mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
	msr cpsr_c, r0
	mov sp, #0x8000

	@ Enabling caching (L1) and brach pridiction
	mrc p15,0,r0,c1,c0,0

	orr r0,#SCTLR_ENABLE_BRANCH_PREDICTION
	orr r0,#SCTLR_ENABLE_DATA_CACHE
	orr r0,#SCTLR_ENABLE_INSTRUCTION_CACHE

	mcr p15,0,r0,c1,c0,0

	@ Enabling VFPU
	mrc p15, #0, r1, c1, c0, #2

	// enable full access for p10,11
	orr r1, r1, #(0xf << 20)
	mcr p15, #0, r1, c1, c0, #2
	mov r1, #0

	// flush prefetch buffer because of FMXR below
	mcr p15, #0, r1, c7, c5, #4
	// and CP 10 & 11 were only just enabled

	// Enable VFP itself
	mov r0,#0x40000000
	fmxr FPEXC, r0

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
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r0, #16

	b c_hw_setup

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


.globl _init_mmu
_init_mmu:
	mov r2,#0

	mcr p15,0,r2,c7,c7,0 ;@ invalidate caches
	mcr p15,0,r2,c8,c7,0 ;@ invalidate tlb
	mcr p15,0,r2,c7,c10,4 ;@ DSB ??

	mvn r2,#0
	@ bic r2,#0xc
	mcr p15,0,r2,c3,c0,0 ;@ domain

	mcr p15,0,r0,c2,c0,0 ;@ tlb base
	mcr p15,0,r0,c2,c0,1 ;@ tlb base

	@ c1, Control Register ( Used to enable/disable/config various things )
	@ such an interrupt vector address, progream flow prediction etc but
	@ for us it does MMU config and enabling also
	mrc p15,0,r2,c1,c0,0
	orr r2,r2,r1
	mcr p15,0,r2,c1,c0,0

	mov pc, lr

