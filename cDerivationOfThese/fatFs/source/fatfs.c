#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <emmc.h>
#include <jtag.h>
#include <stdOutput.h>
#include <libfatfs.h>

void frameBufferSetup(int width, int height, int bitDepth){
    uint32_t error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
    }

    else {
        // error handler
        gpioBlink(200, 10);
		// delay(1000);
    }
}

void kernel_main() {
	// setLEDasOutput();
	_enable_interrupts();
	frameBufferSetup(1024, 768, 16);
	setStartPosition(0,0);
	setCursor(0);
	// gpioBlink(50, 100);
	emmcInit();	
	delay(1000);

	uint8_t sdCardReadBuffer[512];
	emmcSendData(READ_SINGLE, 0x2020, (uint32_t*)&sdCardReadBuffer);
	uint32_t sd[128];
	my_memcpy((uint8_t*)&sd, (uint8_t*)&sdCardReadBuffer, 512, 0);
	for (uint8_t i=0; i < 128; i = i + 8) {
		printf("%i: \t", i);
		printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sd[i], sd[i+1], sd[i+2], sd[i+3], sd[i+4], sd[i+5], sd[i+6], sd[i+7]);
	}
	
	while(1){

	}
	
	
}

void kernel_main2() {

	// jtagInit();
	// setLEDasOutput();
// gpioBlink(200, 10);

	// gpioBlink(50, 100);
	
	_enable_interrupts();
	// volatile int toggledOnce =1;
	frameBufferSetup(1024, 768, 16);
	setStartPosition(0,0);
	setCursor(0);
	// printf("Emmc Init Done\n");
	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	emmcInit();
	// printf("Emmc Init Done\n");
	delay(1000);

	readMBR();
	// printf("Mbr sign %x\n", masterBootRecord.MBR_bootSignature);
	// printf("Partition type %x\n", masterBootRecord.partitionEntries[0].partitionType);
	// printf("LBA of first sector of partition1 %x\n", masterBootRecord.partitionEntries[0].LBAOfFirstSector);


	readPartition1BPB();
	//printf(" OEM Name %x \n", partition1.BS_OEMName[0]);
	// printf(" bytes per sector in partition1 %x \n", partition1.BPB_BytesPerSector);
	// printf(" Sectors per cluster %x \n", partition1.BPB_SectorsPerCluster);
	printf(" Number of Reserved Sectors %x \n", partition1.BPB_ReservedSectorsCount);
	// printf(" number of FAT table copies %i \n", partition1.BPB_NumberOfFATs);
	// printf(" Media type %x \n", partition1.BPB_MediaType);
	printf(" Total Sectors 32 %x \n", partition1.BPB_TotalSectors32);
	printf(" Total Sectors in one copy of FAT32 table %x \n", partition1.BPB_SectorsPerFAT32Table);
	printf(" Cluster no. of Root Directory %x\n", partition1.BPB_RootDirectoryCluster);

	readRootDirectory();

	while(1){
		// volatile uint32_t data = emmcControllerBasicStruct1_t -> data;
	}

}

