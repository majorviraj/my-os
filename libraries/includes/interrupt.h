#ifndef __RPIGPIO_H__
#define __RPIGPIO_H__

#define INTERRUPT_CONTROLLER (PERIPHERAL_BASE + 0xB200)

typedef struct interruptControllerStruct {
    volatile unsigned int IRQbasicPending
    volatile unsigned int IRQpending1;
    volatile unsigned int IRQpending2;
    volatile unsigned int FIQcontrol;
    volatile unsigned int enableIRQ1;
    volatile unsigned int enableIRQ2;
    volatile unsigned int enableBasicIRQs;
    volatile unsigned int disableIRQ1;
    volatile unsigned int disableIRQ2;
    volatile unsigned int disableBasicIRQs;
};

static interruptControllerStruct* IRQController = (interruptControllerStruct*) INTERRUPT_CONTROLLER;