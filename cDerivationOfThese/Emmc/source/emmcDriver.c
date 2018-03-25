#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <emmc.h>
#include <jtag.h>
#include <stdOutput.h>

void frameBufferSetup(int width, int height, int bitDepth){
    uint32_t error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
    }

    else {
        // error handler
        gpioBlink(200, 10);
    }
}

void printEmmcDebug() {
	printf("Respnce 0: %x", emmcControllerBasicStruct1_t->responce0);
	printf("Status: %x", emmcControllerBasicStruct1_t->status);
}
void kernel_main() {

	jtagInit();
	// gpioBlink(50, 100);
	setLEDasOutput();
	
	_enable_interrupts();
	// volatile int toggledOnce =1;
	frameBufferSetup(1024, 768, 16);
	setStartPosition(0,0);
	setCursor(0);
	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	// emmcControllerBasicStruct1_t -> cmdtm = 0x112A0010;
	// emmcControllerBasicStruct1_t -> arg1 = 0x8000;

	emmcInit();
	printf("Emmc Init Done\n");
	delay(1000);
	printEmmcDebug();


	volatile uint32_t dataBlockBuffer[64];
	emmcSendData(17,0, &dataBlockBuffer);
	for (uint8_t i=44; i < 64; i++) {
		uint32_t print_data = dataBlockBuffer[i];
		printf("Number %i is = %x\n", i, print_data);
	}
	while(1){
		// volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}
