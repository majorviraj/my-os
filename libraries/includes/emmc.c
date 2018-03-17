#include <emmc.h>

volatile uint32_t* emmcControllerBasicForceInterrupt = (uint32_t*) (EMMC_CONTROLLER + 64);
volatile uint32_t* spiInterruptSupport = (uint32_t*) (EMMC_CONTROLLER + 0xF0);
volatile uint32_t* slotInterruptAndVersion = (uint32_t*) (EMMC_CONTROLLER + 0xFC);

void emmcGetStatus() {
	uint32_t slotVersion = *slotInterruptAndVersion;

	uint32_t vendor = slotVersion >> 23;

	uint32_t sdVersion = slotVersion & 0x00FF0000;
	sdVersion = sdVersion >> 15;
}

void emmcAllRegisters() {
	volatile static emmcControllerBasicStruct1 allRegs;
	allRegs =  *emmcControllerBasicStruct1_t;
}

uint32_t emmcSendCommand(uint32_t commandIndex, uint32_t arg1) {
	commandIndex = commandIndex << 24;
	emmcControllerBasicStruct1_t -> arg1 = arg1;
	emmcControllerBasicStruct1_t -> cmdtm = commandIndex;

	return emmcControllerBasicStruct1_t -> responce0;
}