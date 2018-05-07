/* FAT FILE SYSTEM DRIVER
*References: For FAT:https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system and 
*					official windows hardware white paper FAT: General Overview
*			 For MBR:https://en.wikipedia.org/wiki/Master_boot_record , this is enough
*Note: This driver for now assumes the FAT drive to be partitioned with an MBR present starting at LBA=0
*Also Logical Block Adresses(LBA) are used. No support for CHS(cylinder head sector) type old files, nobody uses that anymore.
*This driver is specifically designed for the raspberry pi and provides no support for FAT16 or FAT8 type legacy file systems.
*
*
*/

#include <assemblyFunctions.h>
#include <rpiGpio.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <emmc.h>
#include <jtag.h>
#include <stdOutput.h>
#include <emmc.h>
#include <libfatfs.h>


void readMBR() {
	volatile uint8_t sdCardReadBuffer[512];
	// mbrBuffer[511] = 0x52;
	emmcSendData(READ_SINGLE, 0, (uint32_t*)&sdCardReadBuffer);
	// masterBootRecord = (masterBootRecord_t*)&mbrBuffer; //Shallow copy, did not work
	my_memcpy(&masterBootRecord, &sdCardReadBuffer,512, 0); //Deep Copy, this works
	// printf("new mbr sign %x\n", masterBootRecord_t.MBR_bootSignature);
	// printf("mbr buffer 510 sign %x\n", mbrBuffer[510]);
	// printf("mbr buffer 0x1c2 sign %x\n", mbrBuffer[0x1c2]);
	// printf("mbr buffer 511 sign %x\n", mbrBuffer[511]);
	// printf("size of structure is: %i\n", sizeof(masterBootRecord));


}

void readPartition1BPB() {
	const uint8_t sdCardReadBuffer[512];
	emmcSendData(READ_SINGLE, masterBootRecord.partitionEntries[0].LBAOfFirstSector , (uint32_t*)&sdCardReadBuffer);
	
	// for(int i =0; i<90; i++) {
	// 	printf("%x ", sdCardReadBuffer[i]);
	// 	if(i%9 == 0) {
	// 		printf("\n");
	// 	}
	// }
	//my_memcpy(&partition1, &sdCardReadBuffer, 90); //0x5a
	for(int i =0; i<8; i++) {
		partition1.BS_OEMName[i] = sdCardReadBuffer[i+3];

	}
	partition1.BPB_BytesPerSector = sdCardReadBuffer[0x00b] + (sdCardReadBuffer[0x00c]<<8);
	partition1.BPB_SectorsPerCluster = sdCardReadBuffer[0x00d];
	partition1.BPB_ReservedSectorsCount = sdCardReadBuffer[0x00e] +(sdCardReadBuffer[0x00f]<<8);
	partition1.BPB_NumberOfFATs = sdCardReadBuffer[0x010];
	partition1.BPB_RootEntryCount = sdCardReadBuffer[0x011] + (sdCardReadBuffer[0x012]<<8);
	partition1.BPB_TotalSectorsFAT16 = sdCardReadBuffer[0x013] + (sdCardReadBuffer[0x014]<<8);
	partition1.BPB_MediaType = sdCardReadBuffer[0x015];
	partition1.BPB_SectorsPerTrack = sdCardReadBuffer[0x018] + (sdCardReadBuffer[0x019]<<8);
	partition1.BPB_TotalSectors32 = sdCardReadBuffer[0x020] + (sdCardReadBuffer[0x021]<<8) + (sdCardReadBuffer[0x022]<<16) + (sdCardReadBuffer[0x023]<<24);
	partition1.BPB_SectorsPerFAT32Table = sdCardReadBuffer[0x024] + (sdCardReadBuffer[0x025]<<8) + (sdCardReadBuffer[0x026]<<16) + (sdCardReadBuffer[0x027]<<24);
	partition1.BPB_RootDirectoryCluster = sdCardReadBuffer[0x02c] + (sdCardReadBuffer[0x02d]<<8) + (sdCardReadBuffer[0x02e]<<16) + (sdCardReadBuffer[0x02f]<<24);
	partition1.BPB_FSInfo = sdCardReadBuffer[0x030];
	partition1.BPB_BackupBootSec = sdCardReadBuffer[0x032];
	partition1.BS_BootSig = sdCardReadBuffer[0x042];

	// printf("bytes per sector %x\n", sdCardReadBuffer[0x00b]);
	// printf("oem name 0 %x\n", sdCardReadBuffer[3]);
	// printf("oem name 1 %x\n", sdCardReadBuffer[4]);
}

void readRootDirectory() {
	volatile uint8_t sdCardReadBuffer[512];
	//const uint8_t sdCardReadBuffer2[512];


	uint32_t rootDirectoryClusterNumber = masterBootRecord.partitionEntries[0].LBAOfFirstSector
											// + partition1.BPB_RootDirectoryCluster NOT SURE IF I HAVE TO ADD THIS OR NOT, COULDNT INTERPRET FROM THE DOCUMENTATION
											+ partition1.BPB_ReservedSectorsCount
											+ (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table);
	emmcSendData(READ_SINGLE, rootDirectoryClusterNumber, (uint32_t*)&sdCardReadBuffer);
	
	

	//memcpy not working for just first 32 bytes here as well, so deep copying manually
	// for(int i =0 ;i<11; i++) {
	// 	rootDirectory.name[i] = sdCardReadBuffer[i];
	// }
	// rootDirectory.attribute = sdCardReadBuffer[0x0b];
	// rootDirectory.creationTimeInTenths = sdCardReadBuffer[0x0d];
	// rootDirectory.createTime = sdCardReadBuffer[0x0e] + (sdCardReadBuffer[0x0f]<<8);
	// rootDirectory.createDate = sdCardReadBuffer[0x10] + (sdCardReadBuffer[0x11]<<8);
	// rootDirectory.firstClusterHigh = sdCardReadBuffer[0x14] + (sdCardReadBuffer[0x15]<<8);
	// rootDirectory.firstClusterLow = sdCardReadBuffer[0x1a] + (sdCardReadBuffer[0x1b]<<8);
	// rootDirectory.size = sdCardReadBuffer[0x1c] + (sdCardReadBuffer[0x1d]<<8) + (sdCardReadBuffer[0x1e]<<16) + (sdCardReadBuffer[0x1f]<<24);

	printf("***Root Directory***");
	my_memcpy(&rootDirectory, &sdCardReadBuffer, 32, 0);
	printf("\n Name: %c", rootDirectory.name[0]);
	printf("%c", rootDirectory.name[1]);
	printf("%c", rootDirectory.name[2]);
	printf("%c       ", rootDirectory.name[3]);
	printf("Attribute: %x       ", rootDirectory.attribute);
	printf("FirstCluster low: %x      ",rootDirectory.firstClusterLow);
	printf("FirstCluster high: %x\n",rootDirectory.firstClusterHigh);

	printf("***Other Directories and Files***\n");
	directoryEntry_t entries[15];
	for(int k=0; k<15; k++) {
		my_memcpy(&entries[k], &sdCardReadBuffer, 32, (k+1)*32);
		printf("Name: %c", entries[k].name[0]);
		printf("%c", entries[k].name[1]);
		printf("%c", entries[k].name[2]);
		printf("%c       ", entries[k].name[3]);
		printf("Attribute: %x       ", entries[k].attribute);
		/*********************************************************
		*REMEMBER CLUSTER NUMBERING BEGINS AT 2, so SUBTRACT 2 from LOW CLUSTER NUMBER
		*FOUND IN THE DIRECTORY ENTRY TO SEEK THE CONTENTS OF THE FILE.
		*/
		printf("FirstCluster low: %x       ",entries[k].firstClusterLow);
		printf("FirstCluster high: %x\n",entries[k].firstClusterHigh);
	}


	// for(int i =0; i<64; i++) {
	// 	printf("%x ", sdCardReadBuffer[i]);
	// 	if(i%11 == 0  && i != 0) {
	// 		printf("\n");
	// 	}
	// }
	// printf("\n");
	// for(int i =0; i<64; i++) {
	// 	printf("%c .", sdCardReadBuffer[i]);
	// 	if(i%11 == 0  && i != 0) {
	// 		printf("\n");
	// 	}
	// }
	
}
	
void my_memcpy(uint8_t* destination, uint8_t* source, uint32_t length, uint32_t sourceOffset) {
	for(uint32_t i=0; i<length; i++) {
		destination[i] = source[i+sourceOffset];
	}

}