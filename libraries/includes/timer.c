#include <timer.h>

static armTimerStruct* ARMrpiTimer = (armTimerStruct*) ARM_TIMER_BASE_ADDRESS;

int timerInit() {
    IRQController -> enableBasicIRQs |= (unsigned int)RPI_BASIC_ARM_TIMER_IRQ;
}
