#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <emmc.h>
 
/* 
void frameBufferSetup(int width, int height, int bitDepth){
    int error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
    }

    else {
        // error handler
    }
}
*/

void kernel_main() {

	jtagInit();
	delayMicro(10000*1000);
	setLEDasOutput();
	
	_enable_interrupts();
	// volatile int toggledOnce =1;

	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	// emmcControllerBasicStruct1_t -> cmdtm = 0x112A0010;
	// emmcControllerBasicStruct1_t -> arg1 = 0x8000;

	uint32_t resp = emmcSendCommand(8, 0x000001AA);

	delayMicro(2000);

	if (resp == 0x01) {
		while (1) {
			delayMicro(1000*1000);
			gpioToggle();
		}
	}
	else {
		while(1) {
			delayMicro(4000*1000);
			gpioToggle();
		}
	}

	// emmcGetStatus();
	// emmcAllRegisters();
	while(1){
		volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}
