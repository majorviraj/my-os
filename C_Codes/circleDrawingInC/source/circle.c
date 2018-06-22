#include <interrupt.h>
#include <assemblyFunctions.h>
#include <rpiGpio.h>
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

	_enable_interrupts();
    char character = 0;
    int x, y = 0;

    frameBufferSetup(1024, 768, 16);
    
    while(1){
        drawCircle(512, 384, 100);
    }
}