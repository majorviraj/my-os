
/* Defines for various cpsr modes
 */
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

.section .text
.align 2

.type interrupt_context_changer %function
.globl interrupt_context_changer
interrupt_context_changer:
	
	sub lr, #4 	;@ Done as due to pipelining lr has the next address 
			;@ of the mode it came from

	push {r0-r12, lr}

	mrs r0, spsr
	cps #(CPSR_MODE_SVR)	;@ Change to SVR mode
	mov r1, sp
	mov r2, lr

	cps $CPSR_MODE_IRQ	;@ Change to IRQ mode
	push {r0-r2}

	mov r0, r1
	bl interruptRequest_asm 	;@ stack hase been changed by the sceduler
					;@ according to the values of the new scheduled task

	pop {r0-r2}

	msr spsr, r0
	cps #CPSR_MODE_SVR	;@ Change to SVR mode
	mov sp, r1
	mov lr, r2

	cps #CPSR_MODE_IRQ	;@ Change to IRQ mode

	pop {r0-r12, lr}		;@ Restore registers and and branch to new task
	movs pc,lr


/*	This is the contents of the stack with the current state of CPU stored.
 *	
 *	LR_SVR	 //R14_SVR is stored which has to be stored, because the control might be in a subroutine in the SVR mode 
 *	SP_SVR	 //R13_SVR is SP of supervisor mode
 *	SPSR_IRQ //This is the SPSR of IRQ which is a copy of the CPSR of SVR mode when the interrupt occured.
 *	LR_IRQ	 //(This is R14_IRQ) IRET ADDRESS to return to where the control was when in the SVR mode before coming to IRQ
 *	R12	 //Unbanked
 *	R11	 //Unbanked
 *	R10	 //Unbanked
 *	R9	 //Unbanked
 *	R8	 //Unbanked
 *	R7	 //Unbanked
 *	R6	 //Unbanked
 *	R5	 //Unbanked
 *	R4	 //Unbanked
 *	R3	 //Unbanked
 *	R2	 //Unbanked
 *	R1	 //Unbanked	
 *	R0	 //Unbanked
 */ 