#include <emmc.h>
#include <assemblyFunctions.h>
#include <timer.h>
#include <stdOutput.h>
#include <rpiGpio.h>
#define BASE_CLOCK			700000000

#define DIVIER_FOR_25MHz	0x00001000
#define TIMEOUT_ERROR		0xdeadbeef

#define ACMD41_ALL			0x50ff8000
#define SD_1_8V_SUPPORT 	(1 << 24)
#define SDHC_SUPPORT 		(1 << 30)

#define DEBUG_LOG 1
volatile uint32_t* emmcControllerBasicForceInterrupt = (uint32_t*) (EMMC_CONTROLLER + 64);
volatile uint32_t* spiInterruptSupport = (uint32_t*) (EMMC_CONTROLLER + 0xF0);
volatile uint32_t* slotInterruptAndVersion = (uint32_t*) (EMMC_CONTROLLER + 0xFC);
struct emmcDevice devEmmc;

uint32_t emmcGPIOSet() {

	volatile uint32_t* gpio = returnGpio();
	// For GPIO Card Detect
	// gpio[GPIO_GPFSEL4] &= ~(7 << (7*3));

	// gpio[GPIO_GPPUD] = 2;
	// delayCycles(300);
	// gpio[GPIO_GPPUDCLK1] = (1 << (47-32));
	// delayCycles(300);
	// gpio[GPIO_GPPUD] = 0;
	// gpio[GPIO_GPPUDCLK1] = 0;
	// gpio[GPIO_GPHEN1] |= (1 << (47-32));

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

uint32_t emmcGetClockDivider(unsigned int base_clock, unsigned int target_rate) {
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
	if ((interrupt & (1 << 1)) == (1 << 1)) {
		clearInterrupt |= (1 << 1);
	}
	if (interrupt & 0x8000) {
		clearInterrupt |= 0xFFFF0000;
	}
	emmcControllerBasicStruct1_t->interrupt = clearInterrupt;
}

uint32_t emmcSendCommand(uint32_t commandIndex, uint32_t arg1, uint32_t count) {
	
	emmcHandleInterrupts();

	uint8_t commandIsACommand = 0;
	if ((uint32_t)(commandIndex & A_COMMAND_ID) == A_COMMAND_ID) {
		commandIsACommand = 1;
		commandIndex &= ~A_COMMAND_ID;
	}
	#if DEBUG_LOG
	printf("Interrupt in command %i: %x\n", commandIndex >> 24, emmcControllerBasicStruct1_t->interrupt);
	#endif
	uint32_t orignalCount = count;
	if (commandIsACommand) {
		commandIsACommand = 0;
		emmcControllerBasicStruct1_t -> arg1 = 0x00000000;
		if (devEmmc.cardRCA == 0) {
			emmcControllerBasicStruct1_t->cmdtm = APP_CMD_NO_RCA;
			for (;((emmcControllerBasicStruct1_t->status & 1) == 1) && count > 0; count--);
			if (count <= 1) {
				#if DEBUG_LOG
				printf("Timeout Error for APP_CMD_NO_RCA\n");
				#endif
				return TIMEOUT_ERROR;
			}
			count = orignalCount;
			delayCycles(10000);
		}
		else{
			emmcControllerBasicStruct1_t->arg1 = (uint32_t)(devEmmc.cardRCA << 16);
			emmcControllerBasicStruct1_t->cmdtm = APP_CMD;
			for (;((emmcControllerBasicStruct1_t->status & 1) == 1) && count > 0; count--);
			if (count <= 1) {
				#if DEBUG_LOG
				printf("Timeout Error APP_CMD\n");
				#endif
				return TIMEOUT_ERROR;
			}
			count = orignalCount;
			delayCycles(10000);
		}
	}
	emmcControllerBasicStruct1_t -> arg1 = arg1;
	emmcControllerBasicStruct1_t -> cmdtm = commandIndex;
	for (;((emmcControllerBasicStruct1_t->status & 1) == 1) && count > 0; count--);
		if (count <= 1) {
			#if DEBUG_LOG
			printf("Timeout Error\n");
			#endif
			return TIMEOUT_ERROR;
		}

	return emmcControllerBasicStruct1_t -> responce0;
}

void emmcSendData(uint32_t command, uint32_t blockAddress, uint32_t* buf) {

	emmcHandleInterrupts();
	emmcControllerBasicStruct1_t->blockSizeCount = (1 << 16) | 512;
	emmcControllerBasicStruct1_t->arg1 = blockAddress;
	emmcControllerBasicStruct1_t->cmdtm = command;
	// delayCycles(5000);
	
	// printf("Interrupt in data**************** %x", emmcControllerBasicStruct1_t->interrupt);
	#if !DEBUG_LOG
	setForeColour(0xFFFF);
	#endif

	while((emmcControllerBasicStruct1_t->interrupt & INT_READ_RDY) != INT_READ_RDY) {
		#if DEBUG_LOG
		printf("Interrupt in data while =  %x", emmcControllerBasicStruct1_t->interrupt);
		#endif
	}
	// #if DEBUG_LOG
	uint32_t test = 0xdeadbeef;
	
	printf("Interrupt in data****************%x\n", test);

	#if DEBUG_LOG
	printf("responce for data  = %x\n", emmcControllerBasicStruct1_t->responce0);
	#endif
	// delayCycles(20000);

	for (uint16_t i = 0;i<128;i++) {
		*buf = emmcControllerBasicStruct1_t->data;
		buf++;
	}
	#if !DEBUG_LOG
	setForeColour(0x0000);
	#endif
	
}

/*
* @brief This function reads data from the specified byte address into the buffer specified.
*        It reads number of bytes of the length of the array
* @param Pointer to the buffer to be filled
* @param Length of the buffer. (should be length of char array)
*/
void emmcReadData(uint32_t* buffer, uint32_t bufferLength, uint64_t address) {
	
	_Bool isBlockAligned, is;
	uint32_t i = 0x200000; 
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
	#if DEBUG_LOG
		printf("Unable to get Frequency for the required Base Clock and Card Clock\n");
	#endif
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

	uint32_t responce = emmcSendCommand(GO_IDLE_STATE, 0, 100000);
	delayMicro(5);
	// send voltage 0x1 = 2.7 - 3.6V
	// pattern = 0xAA
	// printf("hbfkjwebfkwgflwhfliwhlifhilehfilwnclkwnefh");
	responce = emmcSendCommand(SEND_IF_COMMAND, 0x1AA, 100000);
	while(1){
		if ((responce & 0x1FF) != 0x1AA) {
			responce = emmcControllerBasicStruct1_t->responce0;
			delay(10);
		}
		else {
	#if DEBUG_LOG
			printf("Got Correct Responce : %x\n", responce);
	#endif
			break;
		}
	}
	delayCycles(10000);
	// Send command ACMD41
	// responce = emmcSendCommand(SD_OP_COND, 0, 2000001);

	// responce = emmcSendCommand(SD_OP_COND, ACMD41_ALL, 100000);
	// #if DEBUG_LOG
	// printf("Responce ACMD41: %x\n", emmcControllerBasicStruct1_t->responce0);
	// #endif
	// responce = emmcSendCommand(SD_OP_COND, ACMD41_ALL, 100000);
	// #if DEBUG_LOG
	// printf("Responce ACMD41: %x\n", emmcControllerBasicStruct1_t->responce0);
	// #endif
	// responce = emmcSendCommand(SD_OP_COND, ACMD41_ALL, 100000);

	do {
		responce = emmcSendCommand(SD_OP_COND, ACMD41_ALL, 100000);
		#if DEBUG_LOG
		printf("Responce ACMD41: %x\n", emmcControllerBasicStruct1_t->responce0);
		#endif
		delayCycles(10000);
	}while((uint32_t)(emmcControllerBasicStruct1_t->responce0 & (1 << 31)) != (1 << 31));

	responce = emmcControllerBasicStruct1_t -> responce0;

	devEmmc.cardOCR = (responce >> 8) & 0xFFFF;
	devEmmc.cardIsSDHC = (responce >> 30) & 0x1;
	devEmmc.cardSupports18v = (responce >> 24) & 0x1;

	#if DEBUG_LOG
	printf("Responce for ACMD41: %x\n", emmcControllerBasicStruct1_t->responce0);
	printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	#endif
	responce = emmcSendCommand(ALL_SEND_CID, 0, 100000);
	
	delay(5);
	#if DEBUG_LOG
	printf("CID : %x%x%x%x", emmcControllerBasicStruct1_t->responce0, emmcControllerBasicStruct1_t->responce1, emmcControllerBasicStruct1_t->responce2, emmcControllerBasicStruct1_t->responce3);
	#endif

	responce = emmcSendCommand(SEND_RELATIVE_ADDR, 0, 100000);
	delay(5);
	responce = emmcControllerBasicStruct1_t->responce0;

	devEmmc.cardRCA = (responce >> 16) & 0xFFFF;

	#if DEBUG_LOG
	printf ("Card RCA : %x", devEmmc.cardRCA);
	#endif

	// Entering Data state
	responce = emmcSendCommand(CARD_SELECT, (uint32_t)(devEmmc.cardRCA << 16), 100000);

	delay (5);
	#if DEBUG_LOG
	printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);
	#endif

	responce = emmcControllerBasicStruct1_t->blockSizeCount;
	responce = ~(0xFFF);
	responce |= 0x200;
	emmcControllerBasicStruct1_t->blockSizeCount = responce;
	responce = emmcControllerBasicStruct1_t->responce0;

	// emmcSendCommand(SEND_SCR, 0, 1000000);

	// delay(10);

	// #if DEBUG_LOG
	// printf("Responce: %x\n", emmcControllerBasicStruct1_t->responce0);
	// printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	// printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);
	// printf("DATA: %x", emmcControllerBasicStruct1_t->data);
	// printf("**********************************************\n");
	// #endif
	// devEmmc.cardBusWidth = emmcControllerBasicStruct1_t->responce0;
	// devEmmc.cardBusWidth = __builtin_bswap32(devEmmc.cardBusWidth);
	// devEmmc.cardBusWidth = (devEmmc.cardBusWidth >> 16) & 0xf;
	// #if DEBUG_LOG
	// printf("Bus Width = %x", devEmmc.cardBusWidth);
	// #endif
	uint32_t oldImask = emmcControllerBasicStruct1_t -> interruptMask;
	uint32_t newImask = oldImask & ~(1 << 8);
	emmcControllerBasicStruct1_t->interruptMask = newImask;

	emmcSendCommand(SET_BUS_WIDTH, (uint32_t)(devEmmc.cardRCA << 16)|0x2, 100000);
	delay(5);

	uint32_t control0 = emmcControllerBasicStruct1_t -> control0;
	control0 |= 0x2;
	emmcControllerBasicStruct1_t -> control0 = control0;
	emmcControllerBasicStruct1_t -> interruptMask = oldImask;

	#if DEBUG_LOG
	printf("Responce: %x\n", emmcControllerBasicStruct1_t->responce0);
	printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	printf("Interrupt: %x\n", emmcControllerBasicStruct1_t->interrupt);
	printf("**********************************************\n");
	#endif
	delay(5);

	emmcControllerBasicStruct1_t -> interrupt = 0xFFFFFFFF;

	delay(5);
	#if DEBUG_LOG
	printf("Status: %x\n", emmcControllerBasicStruct1_t->status);
	#endif
	
}