#include <timer.h>
#include <interrupt.h>
#include <intTypes.h>

void timerInit( unsigned int load,
				counterDepth bitDepth,
				unsigned int preScallar) {
    
	IRQController -> enableBasicIRQs |= (unsigned int)RPI_BASIC_ARM_TIMER_IRQ;

	ARMrpiTimer->load = load;
	ARMrpiTimer->control = (unsigned int) (bitDepth << 1) |(unsigned int) (preScallar << 2) |(unsigned int)TIMER_CONTROL_INTERRUPT_ENABLE |(unsigned int)TIMER_CONTROL_ENABLE | (unsigned int)TIMER_CONTROL_FREERUNNING_DISABLE;

}

void timerReload(unsigned int reload) {
	ARMrpiTimer->reload = reload;
}