#include <assemblyFunctions.h>
#include <interrupt.h>
#include <intTypes.h>
#include <jtag.h>
#include <emmc.h>
#include <timer.h>
#include <stdOutput.h>
#include <libfatfs.h>
#include <lib_bmp.h>

uint8_t* fileLoc = (uint8_t*)0xF1000;

void frameBufferSetup(int width, int height, int bitDepth){
    uint32_t error = frameBufferInit(width, height, bitDepth);

    if (error == 0) {
        return;
    }

    else {
        // error handler
        // gpioBlink(200, 10);
    }
}

void kernel_main() {

	_enable_interrupts();
	frameBufferSetup(1024, 768, 16);
	setStartPosition(0,0);
	setCursor(0);
	
	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);
	printf("yoyoyoyoyoyoyoyoyoyoyoyoyoyoyo\n");
	printf("\nThe Number 1 (at 0x1000001):%x", *(uint32_t*)0x1000001);
	printf("\nThe Number 2 (at 0x1000002):%x", *(uint32_t*)0x1000002);
	delay(10000);
	while (1) {

	}
	emmcInit();

	delay(1000);
	
	clearScreen();
	setStartPosition(0,0);
	setCursor(0);

	readMBR();
	readPartition1BPB();
	clearScreen();
	setStartPosition(0,0);
	setCursor(0);
	readRootDirectory();


	

	while(1){
	}

}

