#include "interrupt.h"

void __attribute__((interrupt("IRQ"))) interruptRequest() {
    
    //Enable timer IRQ
    IRQController->enableBasicIRQs |= RPI_BASIC_ARM_TIMER_IRQ;
    
    if (IRQController->IRQBasicPending & RPI_BASIC_ARM_TIMER_IRQ) {
    	//Call timer interrupt handler
    }

}