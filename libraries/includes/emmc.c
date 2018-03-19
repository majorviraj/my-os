#include <emmc.h>
#include <assemblyFunctions.h>
#include <timer.h>
#define BASE_CLOCK 700000000

#define A_COMMAND_ID 0x80000000
#define ACMD(x) A_COMMAND_ID | x

#define SD_1_8V_SUPPORT (1 << 30)
#define SDHC_SUPPORT (1 << 24)
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
	
	uint8_t commandIsACommand = 0;
	if (commandIndex & A_COMMAND_ID == A_COMMAND_ID) {
		commandIsACommand = 1;
		commandIndex &= 0xFFFF;
	}
	
	commandIndex = commandIndex << 24;
	emmcControllerBasicStruct1_t -> arg1 = arg1;
	if (commandIsACommand) {
		commandIsACommand = 0;
		uint32_t resp = emmcSendCommand(55, 0);
	}
	commandIndex |= 0x000A0000; // 0x002A0010 for data commands
	emmcControllerBasicStruct1_t -> cmdtm = commandIndex;

	return emmcControllerBasicStruct1_t -> responce0;
}

void emmcInit() {

	// Reset the controller
	uint32_t control1 = emmcControllerBasicStruct1_t -> control1;
	control1 |= (1 << 24);

	// Stop the clock of the card
	// Internal clock
	control1 &= ~(1 << 2);
	//External clock
	control1 &= ~(1 << 0);

	emmcControllerBasicStruct1_t -> control1 = control1;

	// gpioBlink(1000, 10);
	do {
		control1 = emmcControllerBasicStruct1_t -> control1;
	} while((control1 & (0x7 << 24)) == 0);

	// Clear control1
	emmcControllerBasicStruct1_t -> control2 = 0;

	// Reenabling clock
	control1 = emmcControllerBasicStruct1_t -> control1;

	control1 |= (1 << 0);		// Enabling internal clock
	control1 |= 0x0000ffc0;		// Set frequency to 400 khz
	control1 |= (7 << 16);		// Data timeout = TMCLK * 2^10

	emmcControllerBasicStruct1_t -> control1 = control1;

	do {
		control1 = emmcControllerBasicStruct1_t -> control1;
	} while((control1 & 0x2) == 0);

	
	emmcControllerBasicStruct1_t -> control1 |= 0x4;

	delay(2);

	uint32_t responce = emmcSendCommand(0, 0);


	// send voltage 0x1 = 2.7 - 3.6V
	// pattern = 0xAA

	responce = emmcSendCommand(8, 0x1AA);
	do {
		responce = emmcControllerBasicStruct1_t -> responce0;
	} while((responce & 0x1FF) == 0x1AA);
	
	gpioBlink(500, 5);
	// Send command ACMD41
	responce = emmcSendCommand(ACMD(41), 0);

	delay(500);
	responce = emmcSendCommand(ACMD(41), 0x00FF8000 | SD_1_8V_SUPPORT| SDHC_SUPPORT);

	do {
		responce = emmcControllerBasicStruct1_t -> responce0;
	} while(((responce >> 31) & 0x1) == 1);

	gpioBlink(50, 10);

}