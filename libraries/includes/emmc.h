#ifndef __EMMC_H_
#define __EMMC_H_

#include <rpiGpio.h>

// Addres for the controller
// In case of dought do check this first of all
#define EMMC_CONTROLLER (PERIPHERAL_BASE + 0x300000)

typedef struct {
	// Register for the argument 2 of command
    volatile unsigned int arg2;

	// Size and numberof blocks to read/write to the sd card
	volatile unsigned int blockSizeCount;

	// Register for the argument 1 of command
	volatile unsigned int arg1;

	// Command and responce reqister
	volatile unsigned int cmdtm;

	// Responce from card in 4 registers 1,2,3 and 4
	volatile unsigned int responce0;	
	volatile unsigned int responce1;
	volatile unsigned int responce2;
	volatile unsigned int responce3;

	// Data returned by the register
	volatile unsigned int data;

	// Status of the current tranfer
	volatile unsigned int status;
	
	volatile unsigned int 
	volatile unsigned int 
	volatile unsigned int 
	volatile unsigned int 
	volatile unsigned int 
	volatile unsigned int 
	
} emmcControllerBasicStruct_t ;

#endif
