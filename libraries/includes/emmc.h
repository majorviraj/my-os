#ifndef __EMMC_H__
#define __EMMC_H__

#include <rpiGpio.h>
#include <intTypes.h>
// Addres for the controller
// In case of dought do check this first of all
#define EMMC_CONTROLLER (PERIPHERAL_BASE + 0x300000)

typedef struct {
	// Register for the argument 2 of command
    volatile uint32_t arg2;

	// Size and numberof blocks to read/write to the sd card
	volatile uint32_t blockSizeCount;

	// Register for the argument 1 of command
	volatile uint32_t arg1;

	// Command and responce reqister
	volatile uint32_t cmdtm;

	// Responce from card in 4 registers 1,2,3 and 4
	volatile uint32_t responce0;	
	volatile uint32_t responce1;
	volatile uint32_t responce2;
	volatile uint32_t responce3;

	// Data returned by the register
	volatile uint32_t data;

	// Status of the current tranfer
	volatile uint32_t status;
	
	// Configuration bits for Host
	volatile uint32_t control0;
	volatile uint32_t control1;

	// Interrupt Flags
	volatile uint32_t interrupt;

	// Interrupt Flags enable
	volatile uint32_t interruptMask;

	// Interrupt Generation Enable
	volatile uint32_t interruptEnable;
	volatile uint32_t control2;
	
} emmcControllerBasicStruct1;

static emmcControllerBasicStruct1* emmcControllerBasicStruct1_t = (emmcControllerBasicStruct1*) EMMC_CONTROLLER;
volatile uint32_t* emmcControllerBasicForceInterrupt;

typedef struct {

	// Timeout in boot mode
	volatile uint32_t bootTimeout;

	// Debug Bus Configuration
	volatile uint32_t debugSelect;

	// DO NOT USE RESERVED REGISTERS
	volatile uint32_t reserved0;
	volatile uint32_t reserved1;

	// Extention FIFO Registers
	volatile uint32_t extFIFOConfig;
	volatile uint32_t extFIFOEnable;

	// Delay per card clock tuning step
	volatile uint32_t tuneStep;

	// Card clock tuning steps for SDR
	volatile uint32_t tuneStepsSTD;

	// Card clock tuning steps for DDR
	volatile uint32_t tuneStepsDDR;
} emmcControllerBasicStruct2;

static emmcControllerBasicStruct2* emmcControllerBasicStruct2_t = (emmcControllerBasicStruct2*) EMMC_CONTROLLER;

volatile uint32_t* spiInterruptSupport;
volatile uint32_t* slotInterruptAndVersion;


struct emmcDevice {
	uint32_t cardOCR;
	uint32_t cardSupports18v;
	uint32_t cardIsSDHC;
	uint32_t cardRCA;

};
void emmcGetStatus();

void emmcAllRegisters();

uint32_t emmcSendCommand(uint32_t, uint32_t);

void emmcInit();
void emmcSendData(uint32_t, uint32_t, uint32_t*);

#endif
