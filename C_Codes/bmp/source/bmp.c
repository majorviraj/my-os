#include <assemblyFunctions.h>
#include <interrupt.h>
#include <intTypes.h>
#include <jtag.h>
#include <emmc.h>
#include <timer.h>
#include <stdOutput.h>
#include <libfatfs.h>
#include <lib_bmp.h>

void frameBufferSetup(int width, int height, int bitDepth){
    uint32_t error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
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

	// uint8_t sdCardReadBuffe[512];
	// uint8_t sdCardReadBuffer667[512];
	// uint8_t sdCardReadBuffer66[512];
	// delay(1000);
	// emmcSendData(READ_SINGLE, 0x2000, (uint32_t*)&sdCardReadBuffer667);
	// delay(1000);
	// emmcSendData(READ_SINGLE, 0x2000, (uint32_t*)&sdCardReadBuffe);
	// delay(1000);
	// emmcSendData(READ_SINGLE, 0x2000, (uint32_t*)&sdCardReadBuffer66);

	// for (uint8_t i=0; i < 128; i = i + 8) {
	// 	printf("%i: \t", i);
	// 	printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sdCardReadBuffe[i], sdCardReadBuffe[i+1], sdCardReadBuffe[i+2], sdCardReadBuffe[i+3], sdCardReadBuffe[i+4], sdCardReadBuffe[i+5], sdCardReadBuffe[i+6], sdCardReadBuffe[i+7]);
	// }
	
	
	// for (uint8_t i=0; i < 128; i = i + 8) {
	// 	printf("%i: \t", i);
	// 	printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sdCardReadBuffer66[i], sdCardReadBuffer66[i+1], sdCardReadBuffer66[i+2], sdCardReadBuffer66[i+3], sdCardReadBuffer66[i+4], sdCardReadBuffer66[i+5], sdCardReadBuffer66[i+6], sdCardReadBuffer66[i+7]);
	// }
	
	
	// for (uint8_t i=0; i < 128; i = i + 8) {
	// 	printf("%i: \t", i);
	// 	printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sdCardReadBuffer667[i], sdCardReadBuffer667[i+1], sdCardReadBuffer667[i+2], sdCardReadBuffer667[i+3], sdCardReadBuffer667[i+4], sdCardReadBuffer667[i+5], sdCardReadBuffer667[i+6], sdCardReadBuffer667[i+7]);
	// }
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

	delay(15000);
	// printf("getNextClusterFromFAT(5) %x",getNextClusterFromFAT(5));

	uint8_t fileLoc[60978];
	// readFile(, 0, sizeof(fileLoc), &fileLoc);
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
	// renderBmp(&fileLoc, sizeof(fileLoc), 1, 767);

	while(1){
	}

}

