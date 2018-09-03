#include <assemblyFunctions.h>
#include <interrupt.h>
#include <intTypes.h>
#include <jtag.h>
#include <emmc.h>
#include <timer.h>
#include <stdOutput.h>
#include <libfatfs.h>
#include <lib_bmp.h>

uint8_t* fileLocIndia = (uint8_t*)0xF1000;
uint8_t* fileLocMangoos = (uint8_t*)0x200000;


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
	frameBufferSetup(1920, 1080, 16);
	setStartPosition(0,0);
	setCursor(0);
	
	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);
	printf("yoyoyoyoyoyoyoyoyoyoyoyoyoyoyo\n");
	emmcInit();

	delay(1000);
	
	clearScreen();
	setStartPosition(0,0);
	setCursor(0);
	// printf("yoyoyoyoyoyoyoyoyoyoyoyoyoyoyo\n");
	readMBR();
	// printf("Mbr sign %x\n", masterBootRecord.MBR_bootSignature);
	// printf("Partition type %x\n", masterBootRecord.partitionEntries[0].partitionType);
	// printf("LBA of first sector of partition1 %x\n", masterBootRecord.partitionEntries[0].LBAOfFirstSector);

	// printf("Cluster Information: %x", getNextClusterFromFAT(0x4));
	readPartition1BPB();
	//printf(" OEM Name %x \n", partition1.BS_OEMName[0]);
	// printf(" bytes per sector in partition1 %x \n", partition1.BPB_BytesPerSector);
	// printf(" Sectors per cluster %x \n", partition1.BPB_SectorsPerCluster);
	// printf(" Number of Reserved Sectors %x \n", partition1.BPB_ReservedSectorsCount);
	// // printf(" number of FAT table copies %i \n", partition1.BPB_NumberOfFATs);
	// // printf(" Media type %x \n", partition1.BPB_MediaType);
	// printf(" Total Sectors 32 %x \n", partition1.BPB_TotalSectors32);
	// printf(" Total Sectors in one copy of FAT32 table %x \n", partition1.BPB_SectorsPerFAT32Table);
	// printf(" Cluster no. of Root Directory %x\n", partition1.BPB_RootDirectoryCluster);
	// printf("Number of Fats %x\n", partition1.BPB_NumberOfFATs);
	// delay(10000);

	clearScreen();
	setStartPosition(0,0);
	setCursor(0);
	readRootDirectory();

	// printf("getNextClusterFromFAT(4) %x",getNextClusterFromFAT(4));

	// delay(15000);
	// printf("getNextClusterFromFAT(5) %x",getNextClusterFromFAT(5));

	readFile(0xDA2D, 0x0, 0xEE32, (uint8_t*)fileLocIndia);
	readFile(0xEB10, 0x0, 0xD012, (uint8_t*)fileLocMangoos);
	// delay(15000);
	// clearScreen();
	// setStartPosition(0,0);
	// setCursor(0);
	// char buf[1024 + 512];
	// readFile(0x351, 0x0, 1000, buf);
	// for(uint32_t i = 0; i < 50; i++)
	// {
	// 	printf("%c", fileLoc[i]);
	// }
	
	// printf("file Loc %x\n", fileLoc);
	renderBmp((uint8_t*)fileLocIndia, 0xD012, 1, 767);
	renderBmp((uint8_t*)fileLocMangoos, 0xEE32, 512, 500);

	while(1){
	}

}

