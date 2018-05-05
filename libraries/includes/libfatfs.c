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
	memcpy(&masterBootRecord, &sdCardReadBuffer,512); //Deep Copy, this works
	// printf("new mbr sign %x\n", masterBootRecord.MBR_bootSignature);
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
	partition1.BPB_SectorsPerFAT32 = sdCardReadBuffer[0x024] + (sdCardReadBuffer[0x025]<<8) + (sdCardReadBuffer[0x026]<<16) + (sdCardReadBuffer[0x027]<<24);
	partition1.BPB_RootDirectoryCluster = sdCardReadBuffer[0x02c] + (sdCardReadBuffer[0x02d]<<8) + (sdCardReadBuffer[0x02e]<<16) + (sdCardReadBuffer[0x02f]<<24);
	partition1.BPB_FSInfo = sdCardReadBuffer[0x030];
	partition1.BPB_BackupBootSec = sdCardReadBuffer[0x032];
	partition1.BS_BootSig = sdCardReadBuffer[0x042];

	// printf("bytes per sector %x\n", sdCardReadBuffer[0x00b]);
	// printf("oem name 0 %x\n", sdCardReadBuffer[3]);
	// printf("oem name 1 %x\n", sdCardReadBuffer[4]);
}
	
