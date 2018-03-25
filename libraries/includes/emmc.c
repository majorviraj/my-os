#include <emmc.h>
#include <assemblyFunctions.h>
#include <timer.h>
#include <stdOutput.h>
#define BASE_CLOCK	700000000

#define DIVIER_FOR_25MHz	0x00001000
#define A_COMMAND_ID 		0x80000000
#define ACMD(x) A_COMMAND_ID | x

#define SD_1_8V_SUPPORT (1 << 24)
#define SDHC_SUPPORT (1 << 30)
volatile uint32_t* emmcControllerBasicForceInterrupt = (uint32_t*) (EMMC_CONTROLLER + 64);
volatile uint32_t* spiInterruptSupport = (uint32_t*) (EMMC_CONTROLLER + 0xF0);
volatile uint32_t* slotInterruptAndVersion = (uint32_t*) (EMMC_CONTROLLER + 0xFC);
struct emmcDevice devEmmc;

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
	if ((uint32_t)(commandIndex & A_COMMAND_ID) == A_COMMAND_ID) {
		commandIsACommand = 1;
		commandIndex &= 0xFFFF;
	}
	commandIndex = commandIndex << 24;
	if (commandIsACommand) {
		commandIsACommand = 0;
		emmcControllerBasicStruct1_t -> arg1 = 0x00000000;
		emmcControllerBasicStruct1_t->cmdtm = 0x370A0000;
	}
	emmcControllerBasicStruct1_t -> arg1 = arg1;
	commandIndex |= 0x000A0000; // 0x002A0010 for data commands
	emmcControllerBasicStruct1_t -> cmdtm = commandIndex;

	return emmcControllerBasicStruct1_t -> responce0;
}

void emmcSendData(uint32_t commandIndex, uint32_t blockAddress, uint32_t* buf) {
	commandIndex = commandIndex << 24;
	commandIndex |= 0x002A0010;
	emmcControllerBasicStruct1_t->arg1 = blockAddress;
	emmcControllerBasicStruct1_t->cmdtm = commandIndex;
	printf("Command: %x\n", emmcControllerBasicStruct1_t->cmdtm);
	delay(5);
	for (uint16_t i = 0;i<64;i++) {
		*buf = emmcControllerBasicStruct1_t->data;
		buf++;
	}

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

	do {
		control1 = emmcControllerBasicStruct1_t -> control1;
	} while((uint32_t)(control1 & (0x7 << 24)) == 0);

	// Clear control2
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

delay(5);
	// send voltage 0x1 = 2.7 - 3.6V
	// pattern = 0xAA

	responce = emmcSendCommand(8, 0x1AA);
	while(1){
		if ((responce & 0xFF) != 0xAA) {
			responce = emmcControllerBasicStruct1_t->responce0;
			printf("In if Responce : %x", responce);
			delay(1);
		}
		else {
			printf("In else Responce : %x", responce);
			break;
		}
	}

	delay(5);
	printf("Responce for 8: %x\n", emmcControllerBasicStruct1_t->responce0);
	printf("Status for 8: %x\n", emmcControllerBasicStruct1_t->status);
	printf("Interrupt for 8: %x\n", emmcControllerBasicStruct1_t->interrupt);
	// Send command ACMD41
	// responce = emmcSendCommand(ACMD(41), 0);

	delay(500);
	// responce = emmcSendCommand(ACMD(41), SDHC_SUPPORT);


	do {
		responce = emmcControllerBasicStruct1_t -> responce0;
	} while(((responce >> 31) & 0x1) == 1);
	// printf("Argument 1: %x", emmcControllerBasicStruct1_t->arg1);
	// printf("Responce 1: %x", emmcControllerBasicStruct1_t->responce0);

	responce = emmcControllerBasicStruct1_t -> responce0;

	devEmmc.cardIsSDHC = (responce >> 8) & 0xFFFF;
	devEmmc.cardIsSDHC = (responce >> 30) & 0x1;
	devEmmc.cardSupports18v = (responce >> 24) & 0x1;

	// Now Lets change the clock to 25Mhz
	
	// do {
	// 	responce = emmcControllerBasicStruct1_t -> status;
	// } while(responce & 0x3);

	// control1 = emmcControllerBasicStruct1_t -> control1;
	// control1 &= ~(1 << 2);
	// emmcControllerBasicStruct1_t -> control1 = control1;

	delay(2);

	// // Writing the new divider
	// control1 &= ~0xffe0;
	// control1 |= DIVIER_FOR_25MHz;
	// emmcControllerBasicStruct1_t -> control1 = control1;
	// control1 |= (1 << 2);

	// emmcControllerBasicStruct1_t -> control1 = control1;
	delay(2);
	printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	responce = emmcSendCommand(3, 0);
	
	delay(50);
	// printf("Responce: %x\n", emmcControllerBasicStruct1_t->responce0);
	// printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	// printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);
	responce = emmcControllerBasicStruct1_t->responce0;

	devEmmc.cardRCA = (responce >> 16) & 0xFFFF;
	printf("RCA : %x", devEmmc.cardRCA);
	// Entering Data state
	// responce = emmcSendCommand(7, 0);

printf("Responce: %x\n", emmcControllerBasicStruct1_t->responce0);
	printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);

	delay (5);
	printf("Status: %x", emmcControllerBasicStruct1_t->status);
	printf("command: %x", emmcControllerBasicStruct1_t->cmdtm);

	responce = emmcControllerBasicStruct1_t -> responce0;
	// printf("Responce 0: %x", emmcControllerBasicStruct1_t->responce0);
	emmcSendCommand(7, (uint32_t)(devEmmc.cardRCA << 16));

	delay(5);
	

	uint32_t oldImask = emmcControllerBasicStruct1_t -> interruptMask;
	uint32_t newImask = oldImask & ~(1 << 8);

	emmcControllerBasicStruct1_t -> interruptMask = responce;

	emmcSendCommand(ACMD(6), 0x2);
	delay(5);

	uint32_t control0 = emmcControllerBasicStruct1_t -> control0;
	control0 |= 0x2;
	emmcControllerBasicStruct1_t -> control0 = control0;
	emmcControllerBasicStruct1_t -> interruptMask = oldImask;

	delay(100);

	emmcControllerBasicStruct1_t -> interrupt = 0xFFFFFFFF;

	gpioBlink(500, 5);
	
}