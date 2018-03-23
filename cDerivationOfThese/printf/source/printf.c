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

uint32_t cuser[2] = {50,51};
uint32_t* ptr = &cuser;
// char x='d';
int x=-5;
char y='y';
void kernel_main() {
    frameBufferSetup(1024, 768, 16);
    setStartPosition(0,0);
	gpioBlink(50, 20);
    setCursor(1);
    // put('H');
	// put('e');
	// setCursor(20);
    // printf("He");
	// putInt(x);
	printf("Hello World\n");
	putInt(-5);
	printf("Hello World");
	printf("Hello%c World", y);
	// drawCharacter('X',100, 100);
	// for(uint32_t i=0;i<10;i++) {
             printf("test %i", x);
    //     }
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
    
 }

