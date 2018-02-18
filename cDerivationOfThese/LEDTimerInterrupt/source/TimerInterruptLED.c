#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <timer.h>
#include <stdOutput.h>
#include <intTypes.h>
 
 
void frameBufferSetup(int width, int height, int bitDepth){
    int error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
    }

    else {
        // error handler
    }
}


void kernel_main() {
    frameBufferSetup(1366, 768, 16);
    setStartPosition(100,100);
    setCursor(1);
    putChar('x');
    printf("Hello world \t");
    while(1){
        //printf("Hello world \t");
        // for(uint32_t i=0;i<10;i++) {
        //     printf("test %i", i);       
        // }
    }
    

    // setLEDasOutput();
    // int reachedZero=0;
    // int toggledOnce=0;
    // timerInit(0x400, Bit23, TIMER_CONTROL_PRESCALLAR_1);
    
    // if (ARMrpiTimer->count < 10 && toggledOnce == 0 ) {
    //     gpioToggle();
    //     toggledOnce = 1;
    // }
    // if(ARMrpiTimer->count >= 10 && toggledOnce==1) {
    //     toggledOnce = 0;
    // } 

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
