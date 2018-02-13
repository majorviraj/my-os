#include "assemblyFunctions.h"
#include "rpiGpio.h"
#include "interrupt.h"
#include "timer.h"
 
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
        
    gpio[LED_GPFSEL] |= (1 << LED_GPIO_BIT);    
	timerInit(0xFF, Bit16, TIMER_CONTROL_PRESCALLAR_16);


	while(1){

	}

}

