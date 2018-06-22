#include <timer.h>
#include <interrupt.h>
#include <intTypes.h>

void timerInit( unsigned int load,
				counterDepth bitDepth,
				unsigned int preScallar) {
    
	IRQController -> enableBasicIRQs |= (unsigned int)RPI_BASIC_ARM_TIMER_IRQ;

	ARMrpiTimer->load = load ;
	ARMrpiTimer->control |= (bitDepth << 1) |(preScallar) |TIMER_CONTROL_ENABLE |TIMER_CONTROL_FREERUNNING_DISABLE| TIMER_CONTROL_INTERRUPT_ENABLE;
}

void timerReload(unsigned int reload) {
	ARMrpiTimer->reload = reload;
}
