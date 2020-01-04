#include <interrupt.h>
#include <timer.h>
#include <lib_uart.h>

// Legacy interrupt handler !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
// void __attribute__((interrupt("IRQ"))) interruptRequest() {
    
//     //Enable timer IRQ
    
//     if (IRQController->IRQBasicPending & RPI_BASIC_ARM_TIMER_IRQ) {
//     	//Call timer interrupt handler

// 			ARMrpiTimer -> IRQClear = 1;
//     			gpioToggle();
// 			printf("Timer IRQ!!");
//     } else if (IRQController->IRQBasicPending & RPI_BASIC_PENDING_1_IRQ){
	    
// 	    //Interrupt number 29 is Aux_peripheral interrupt. Call the uart_irq_handler function here

// 	    if (IRQController->IRQGPUpending1 & (1<<29)) {
// 		    uart_irq_handler();
// 	    }
//     }

// }

/*
 * New interrupt handler, called from the asm interrupt vector.
 * Here the pending interrupts are figured out and corresponding handlers are called
 * On the timer interrupt, the scheduler is supposed to be called, which loads the
 * right context into the IRQ stack.
 */

void interruptRequest_asm(uint32_t* context_stack_pointer) {
    
    //Enable timer IRQ
    
    if (IRQController->IRQBasicPending & RPI_BASIC_ARM_TIMER_IRQ) {
    	//Call timer interrupt handler

			ARMrpiTimer -> IRQClear = 1;
			// uart_putchar('\n');
			// uart_putchar('9');
			// uart_putchar(*(context_stack_pointer + ));
    			scheduler(context_stack_pointer - 1);
			printf('\n #');
			// printf("Timer IRQ!!");
			
    } else if (IRQController->IRQBasicPending & RPI_BASIC_PENDING_1_IRQ){
	    
	    //Interrupt number 29 is Aux_peripheral interrupt. Call the uart_irq_handler function here

	    if (IRQController->IRQGPUpending1 & (1<<29)) {
		    uart_irq_handler();
	    }
    } else 
    {
	    return;
    }

}

void __attribute__((interrupt("FIQ"))) fastInterrupt() {

}
void __attribute__((interrupt("UNDEF"))) undefinedInstruction() {

}
void __attribute__((interrupt("SWI"))) softwareInterrupt() {

}
void __attribute__((interrupt("ABORT"))) prefetchAbort() {

}
