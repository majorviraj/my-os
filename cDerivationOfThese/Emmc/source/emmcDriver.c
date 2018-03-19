#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <emmc.h>
#include <jtag.h>
 
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
	// gpioBlink(50, 100);
	setLEDasOutput();
	
	_enable_interrupts();
	// volatile int toggledOnce =1;

	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	// emmcControllerBasicStruct1_t -> cmdtm = 0x112A0010;
	// emmcControllerBasicStruct1_t -> arg1 = 0x8000;

	emmcInit();
	delayMicro(2000);

	// emmcGetStatus();
	// emmcAllRegisters();
	while(1){
		volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}
