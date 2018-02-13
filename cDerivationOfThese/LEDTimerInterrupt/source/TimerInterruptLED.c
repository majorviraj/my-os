#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
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

    _enable_interrupts();
        
    setLEDasOutput() ;
	timerInit(0xFF, Bit16, TIMER_CONTROL_PRESCALLAR_16);
	
	IRQController->enableBasicIRQs |= RPI_BASIC_ARM_TIMER_IRQ;

	while(1){

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
