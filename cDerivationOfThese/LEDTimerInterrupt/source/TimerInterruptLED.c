#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
 
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
    // setLEDasOutput();
	setLEDasOutput();

	volatile int toggledOnce =1;

	ARMrpiTimer -> load = 100000;
	ARMrpiTimer -> control = (unsigned int)0x8A;
	volatile unsigned int count = 500000;
	// volatile unsigned int tim = 0;
// while(1){
// 	for (tim = 0; tim < 500000; tim++);

//     	gpioToggle();

//     for (tim = 0; tim < 500000; tim++);
        
// 		gpioToggle();

// }
    while(1){
			if (ARMrpiTimer -> count < 1000 && toggledOnce == 0 ) {
				gpioToggle();
				toggledOnce = 1;
			}
			if(ARMrpiTimer ->count >= 1000 && toggledOnce==1) {
				toggledOnce = 0;
			}
	}

}

// void kernel_main() {

//     _enable_interrupts();
        
//     setLEDasOutput();

// while(1) {

// 	for (unsigned int tim = 0; tim < 500000; tim++);

// 	gpioToggle();

// 	timerInit(0xFF, Bit16, TIMER_CONTROL_PRESCALLAR_16);

// }

// }
