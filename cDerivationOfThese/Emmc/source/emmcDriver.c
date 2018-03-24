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

void kernel_main() {

	jtagInit();
	// gpioBlink(50, 100);
	setLEDasOutput();
	
	_enable_interrupts();
	// volatile int toggledOnce =1;
	frameBufferSetup(1024, 768, 16);
	setStartPosition(0,0);
	setCursor(1);
	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	// emmcControllerBasicStruct1_t -> cmdtm = 0x112A0010;
	// emmcControllerBasicStruct1_t -> arg1 = 0x8000;

	// emmcInit();
	printf("Emmc Init Doneiijij\n");


	uint32_t dataBlockBuffer[64] = {0};
	dataBlockBuffer[63] = 0x674865;
	// emmcSendData(17,0, &dataBlockBuffer);
	
	putInt(dataBlockBuffer[63]);
	while(1){
		volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}
