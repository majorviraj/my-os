#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "rpiGpio.h"

#define INTERRUPT_CONTROLLER (PERIPHERAL_BASE + 0xB200)

#define RPI_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define RPI_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define RPI_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define RPI_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define RPI_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define RPI_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define RPI_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define RPI_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

typedef struct {
    volatile unsigned int IRQBasicPending;
    volatile unsigned int IRQGPUpending1;
    volatile unsigned int IRQGPUpending2;
    volatile unsigned int FIQcontrol;
    volatile unsigned int enableIRQ1;
    volatile unsigned int enableIRQ2;
    volatile unsigned int enableBasicIRQs;
    volatile unsigned int disableIRQ1;
    volatile unsigned int disableIRQ2;
    volatile unsigned int disableBasicIRQs;
} interruptControllerStruct ;

static interruptControllerStruct* IRQController = (interruptControllerStruct*) INTERRUPT_CONTROLLER;

void __attribute__((interrupt("IRQ"))) interruptRequest() ;
void __attribute__((interrupt("FIQ"))) fastInterrupt() ;
void __attribute__((interrupt("UNDEF"))) undefinedInstruction() ;
void __attribute__((interrupt("SWI"))) softwareInterrupt() ;
void __attribute__((interrupt("ABORT"))) prefetchAbort() ;

#endif
