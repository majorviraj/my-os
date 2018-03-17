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

	timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	emmcControllerBasicStruct1_t -> cmdtm |= 0x24200010;

	emmcGetStatus();
	emmcAllRegisters();
	while(1){
		volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}
