#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
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
	setLEDasOutput();
	while(1) {
		
		// for (volatile unsigned int tim = 0; tim < 500000; tim++);
		delayMicro(1000*1000);
		gpioToggle();
	}

}
