#ifndef _TIMER_H_
#define _TIMER_H_

#include <interrupt.h>
#include <assemblyFunctions.h>
#define delay(timeMs) delayMicro(timeMs*1000);

#define ARM_TIMER_BASE_ADDRESS (0x20000000 + 0xB400)

#define TIMER_CONTROL_23BITS        		(1 << 1)
#define TIMER_CONTROL_16BITS				(0 << 1)

#define TIMER_CONTROL_PRESCALLAR_1 			(0 << 2)
#define TIMER_CONTROL_PRESCALLAR_16 		(1 << 2)
#define TIMER_CONTROL_PRESCALLAR_256 		(2 << 2)

#define TIMER_CONTROL_INTERRUPT_ENABLE 		(1 << 5)
#define TIMER_CONTROL_INTERRUPT_DISABLE 	(0 << 5)

#define TIMER_CONTROL_ENABLE 				(1 << 7)
#define TIMER_CONTROL_DISABLE 				(0 << 7)

#define TIMER_CONTROL_FREERUNNING_ENABLE 	(1 << 9)
#define TIMER_CONTROL_FREERUNNING_DISABLE 	(0 << 9)

#define LOAD_VALUE_10S_1_PRESCALLAR			0x9505F9UL
#define LOAD_VALUE_1S_1_PRESCALLAR			0x09505FUL
#define LOAD_VALUE_1S_16_PRESCALLAR 		0xEE6B28
 

typedef struct {
    // Value to be counted from (!!this is a down counter!!) 
    volatile unsigned int load;
    
    // The actual value of the timer
    volatile unsigned int count;

    /* The control register for the timer
        0: Reserved
        1: 0 - 16 bit
           1 - 23 bit
      2-3: PreScallar
        5: Timer Interrupt
        7: Timer enable
        8: Debug halted mode
        9: Freerunning enable
    23-16: Freerunning prescallar */
    volatile unsigned int control;

    // When writing anything here interrupt pending is cleard
    volatile unsigned int IRQClear;

    // Status for interrupt pending bit
    volatile unsigned int rawIRQ;

    // Logical and of interrupt pending and interrupt enable
    volatile unsigned int maskedIRQ;

    // this value is filled in load when timer reaches 0
    volatile unsigned int reload;

    // This can be used to divide the freerunning clock further
    // timer_clock = apb_clock/(pre_divider+1)
    volatile unsigned int preDivide;
    
    // The value of the free running counter (this is a up counter)
    volatile unsigned int freeRuning;
} armTimerStruct;

typedef enum counterLength {Bit16=0, Bit23=1} counterDepth;
void timerInit(	unsigned int,
				enum counterLength,		
				unsigned int);

void reload(unsigned int);

static armTimerStruct* ARMrpiTimer = (armTimerStruct*) ARM_TIMER_BASE_ADDRESS;

#endif