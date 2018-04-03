#include <emmc.h>
#include <assemblyFunctions.h>
#include <timer.h>
#include <stdOutput.h>
#include <rpiGpio.h>
#define BASE_CLOCK	700000000

#define DIVIER_FOR_25MHz	0x00001000

#define SD_1_8V_SUPPORT (1 << 24)
#define SDHC_SUPPORT (1 << 30)
volatile uint32_t* emmcControllerBasicForceInterrupt = (uint32_t*) (EMMC_CONTROLLER + 64);
volatile uint32_t* spiInterruptSupport = (uint32_t*) (EMMC_CONTROLLER + 0xF0);
volatile uint32_t* slotInterruptAndVersion = (uint32_t*) (EMMC_CONTROLLER + 0xFC);
struct emmcDevice devEmmc;

uint32_t emmcGPIOSet() {

	volatile uint32_t* gpio = returnGpio();
	// For GPIO Card Detect
	gpio[GPIO_GPFSEL4] &= ~(7 << (7*3));

	gpio[GPIO_GPPUD] = 2;
	delayCycles(300);
	gpio[GPIO_GPPUDCLK1] = (1 << (47-32));
	delayCycles(300);
	gpio[GPIO_GPPUD] = 0;
	gpio[GPIO_GPPUDCLK1] = 0;
	gpio[GPIO_GPHEN1] |= (1 << (47-32));

	// For GPIO_CLOCK and GPIO_CMD
	gpio[GPIO_GPFSEL4] |= (7 << (8*3))|(7 << (9*3));
	gpio[GPIO_GPPUD] = 2;
	delayCycles(300);
	gpio[GPIO_GPPUDCLK1] = (1 << (48-32))|(1 << (49-32));
	delayCycles(300);
	gpio[GPIO_GPPUD] = 0;
	gpio[GPIO_GPPUDCLK1] = 0;

	// For GPIO DAT0, DAT1, DAT2, DAT3
	gpio[GPIO_GPFSEL5] |= (7 << (0*3)) | (7 << (1*3)) | (7 << (2*3)) | (7 << (3*3));
	gpio[GPIO_GPPUD] = 2;
	delayCycles(300);
	gpio[GPIO_GPPUDCLK1] = (1 << (53-32))|(1 << (52-32))|(1 << (51-32))|(1 << (50-32));
	delayCycles(300);
	gpio[GPIO_GPPUD] = 0;
	gpio[GPIO_GPPUDCLK1] = 0;

}

uint32_t emmcGetClockDivider(unsigned int base_clock, unsigned int target_rate)
{
	// MATH - HOW DOES IT WORK!?
	unsigned int targetted_divisor = 0;
	if(target_rate > base_clock)
		targetted_divisor = 1;
	else
	{
		targetted_divisor = base_clock / target_rate;
		if(base_clock % target_rate)
			targetted_divisor--;
	}
	
	// Find the first set bit 
	unsigned int divisor = -1;
	unsigned int first_bit;
	for(first_bit = 31; first_bit >= 0; first_bit--)
	{
		unsigned int bit_test = (1 << first_bit);
		if(targetted_divisor & bit_test)
		{
			divisor = first_bit;
			targetted_divisor &= ~bit_test;
			if(targetted_divisor)
				divisor++; // The divisor is not power of two, increase to fix

			break; // Found it!
		}
	}

	if(divisor == -1)
		divisor = 31;
	if(divisor >= 32)
		divisor = 31;

	if(divisor != 0)
		divisor = (1 << (divisor -1));

	if(divisor >= 0x400)
		divisor = 0x3FF;

	unsigned int freq_select = divisor & 0xFF;
	unsigned int upper_bits = (divisor >> 8) & 0x3;
	unsigned int ret = (freq_select << 8) | (upper_bits << 6) | (0 << 5);

	// For debugging
	//int denominator = -1;
	//if(divisor != 0)
	//	denominator = divisor * 2;
	//
	//int actual_clock = base_clock / denominator;

	return ret;
}

void emmcGetStatus() {
	uint32_t slotVersion = *slotInterruptAndVersion;

	uint32_t vendor = slotVersion >> 23;

	uint32_t sdVersion = slotVersion & 0x00FF0000;
	sdVersion = sdVersion >> 15;
}

void emmcHandleInterrupts() {
	uint32_t interrupt = emmcControllerBasicStruct1_t->interrupt;
	uint32_t clearInterrupt = 0;
	if ((interrupt & (1 << 6)) == (1 << 6)) {
		clearInterrupt |= (1 << 6);
	}
	if ((interrupt & (1 << 0)) == (1 << 0)) {
		// Command complete interrupt
		clearInterrupt |= (1 << 0);
	}
	emmcControllerBasicStruct1_t->interrupt = clearInterrupt;
}

uint32_t emmcSendCommand(uint32_t commandIndex, uint32_t arg1) {
	
	emmcHandleInterrupts();

	printf("Interrupt in command %i: %x\n", commandIndex >> 24, emmcControllerBasicStruct1_t->interrupt);
	uint8_t commandIsACommand = 0;
	if ((uint32_t)(commandIndex & A_COMMAND_ID) == A_COMMAND_ID) {
		commandIsACommand = 1;
		commandIndex &= ~A_COMMAND_ID;
	}

	if (commandIsACommand) {
		commandIsACommand = 0;
		emmcControllerBasicStruct1_t -> arg1 = 0x00000000;
		if (devEmmc.cardRCA == 0) {
			emmcControllerBasicStruct1_t->cmdtm = APP_CMD_NO_RCA;
		}
		else{
			emmcControllerBasicStruct1_t->cmdtm = APP_CMD;
		}
	}
	emmcControllerBasicStruct1_t -> arg1 = arg1;
	emmcControllerBasicStruct1_t -> cmdtm = commandIndex;

	return emmcControllerBasicStruct1_t -> responce0;
}

void emmcSendData(uint32_t commandIndex, uint32_t blockAddress, uint32_t* buf) {
	commandIndex = commandIndex << 24;
	commandIndex |= 0x002A0010;
	emmcControllerBasicStruct1_t->arg1 = blockAddress;
	emmcControllerBasicStruct1_t->cmdtm = commandIndex;
	delay(5);
	for (uint16_t i = 0;i<64;i++) {
		*buf = emmcControllerBasicStruct1_t->data;
		buf++;
	}

}

void emmcInit() {

	emmcGPIOSet();
	// Reset the controller
	
	uint32_t control1 = emmcControllerBasicStruct1_t -> control1;
	control1 |= (1 << 24);

	// Stop the clock of the card
	// Internal clock
	control1 &= ~(1 << 2);
	//External clock
	control1 &= ~(1 << 0);

	emmcControllerBasicStruct1_t -> control1 = control1;

	// while((emmcControllerBasicStruct1_t->status & (1 << 16)) != (1 << 16));
	do {
		control1 = emmcControllerBasicStruct1_t -> control1;
	} while((uint32_t)(control1 & (0x7 << 24)) != 0);

	// wait for card to be detected
	while((emmcControllerBasicStruct1_t->status & (1 << 16)) != (1 << 16));

	// Clear control2
	emmcControllerBasicStruct1_t -> control2 = 0;

	// Reenabling clock
	control1 = emmcControllerBasicStruct1_t -> control1;

	control1 |= (1 << 0);							// Enabling internal clock
	uint32_t freqForId = emmcGetClockDivider(BASE_CLOCK, 25000000);
	if (freqForId == -1) {
		printf("Unable to get Frequency for the required Base Clock and Card Clock\n");
	}
	
	control1 |= freqForId;	// Set frequency to 400 khz
	control1 |= (7 << 16);							// Data timeout = TMCLK * 2^10

	emmcControllerBasicStruct1_t -> control1 = control1;

	do {
		control1 = emmcControllerBasicStruct1_t -> control1;
	} while((control1 & 0x2) != 0);
	
	delay(5);

	emmcControllerBasicStruct1_t -> control1 |= 0x4;

	delay(5);

	// Disable interrupts
	emmcControllerBasicStruct1_t->interruptEnable = 0xFFFFFFFF;
	emmcControllerBasicStruct1_t->interruptMask = 0xFFFFFFFF;

	uint32_t responce = emmcSendCommand(GO_IDLE_STATE, 0);

	// send voltage 0x1 = 2.7 - 3.6V
	// pattern = 0xAA

	responce = emmcSendCommand(SEND_IF_COMMAND, 0x1AA);
	while(1){
		if ((responce & 0x1FF) != 0x1AA) {
			responce = emmcControllerBasicStruct1_t->responce0;
			delay(10);
		}
		else {
			printf("Got Correct Responce : %x\n", responce);
			break;
		}
	}

	// Send command ACMD41
	responce = emmcSendCommand(SD_OP_COND, 0);

	delay(1000);
	
	responce = emmcSendCommand(SD_OP_COND, 0x00ff8000 |SDHC_SUPPORT);

	do {
		responce = emmcControllerBasicStruct1_t -> responce0;
	} while(((responce >> 31) & 0x1) == 1);
	printf("Argument 1: %x", emmcControllerBasicStruct1_t->arg1);
	printf("Responce 1: %x", emmcControllerBasicStruct1_t->responce0);
	printf("Status for ACMD41: %x", emmcControllerBasicStruct1_t->status);

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

	responce = emmcSendCommand(3, 0);
	
	delay(50);
	// printf("Responce: %x\n", emmcControllerBasicStruct1_t->responce0);
	// printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	// printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);
	responce = emmcControllerBasicStruct1_t->responce0;

	devEmmc.cardRCA = (responce >> 16) & 0xFFFF;
	// Entering Data state
	// responce = emmcSendCommand(7, 0);

	delay (5);

	responce = emmcControllerBasicStruct1_t -> responce0;
	// printf("Responce 0: %x", emmcControllerBasicStruct1_t->responce0);
	emmcSendCommand(7, (uint32_t)(devEmmc.cardRCA << 16));

	delay(5);
	

	uint32_t oldImask = emmcControllerBasicStruct1_t -> interruptMask;
	uint32_t newImask = oldImask & ~(1 << 8);

	emmcControllerBasicStruct1_t -> interruptMask = responce;

	// emmcSendCommand(ACMD(6), 0x2);
	delay(5);

	uint32_t control0 = emmcControllerBasicStruct1_t -> control0;
	control0 |= 0x2;
	emmcControllerBasicStruct1_t -> control0 = control0;
	emmcControllerBasicStruct1_t -> interruptMask = oldImask;

	delay(100);

	emmcControllerBasicStruct1_t -> interrupt = 0xFFFFFFFF;

	// gpioBlink(500, 5);
	
}