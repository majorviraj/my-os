#include "interrupt.h"
 
void __attribute__((interrupt("IRQ"))) interruptRequest() {
    
    //Enable timer IRQ
    IRQController->enableBasicIRQs |= RPI_BASIC_ARM_TIMER_IRQ;
    
    if (IRQController->IRQBasicPending & RPI_BASIC_ARM_TIMER_IRQ) {
    	//Call timer interrupt handler
    	gpioToggle();
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
