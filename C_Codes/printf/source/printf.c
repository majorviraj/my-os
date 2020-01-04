#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <timer.h>
#include <stdOutput.h>
#include <intTypes.h>
 
 
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

int x=0xDEADBEEF;
char* y=" 7ololololololololollolol ";
void kernel_main() {
    frameBufferSetup(1920, 1080, 16);
    setStartPosition(0,0);
    setCursor(0);
	printf("Hello World\n");
	putInt(-5);
	printf("Hello World");
	printf("Hello%s World", y);
	// drawCharacter('X',100, 100);
	// for(uint32_t i=0;i<10;i++) {
             printf("test %x", x);
    //     }
    while(1){
        //printf("Hello world \t");
        // for(uint32_t i=0;i<10;i++) {
        //     printf("test %i", i);       
        // }
    }
    
}

