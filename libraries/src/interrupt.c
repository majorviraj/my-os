#include <interrupt.h>
#include <timer.h>

void __attribute__((interrupt("IRQ"))) interruptRequest() {
    
    //Enable timer IRQ
    
    if (IRQController->IRQBasicPending & RPI_BASIC_ARM_TIMER_IRQ) {
    	//Call timer interrupt handler

			ARMrpiTimer -> IRQClear = 1;
    			gpioToggle();
			printf("Timer IRQ!!");
    } else if (IRQController->IRQBasicPending & RPI_BASIC_PENDING_1_IRQ){
	    
	    //Interrupt number 29 is Aux_peripheral interrupt. Call the uart_irq_handler function here

	    if (IRQController->IRQGPUpending1 & (1<<29)) {
		    uart_irq_handler();
	    }
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
