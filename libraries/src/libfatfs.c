/* FAT FILE SYSTEM DRIVER
*References: For FAT:https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system and 
*					official windows hardware white paper FAT: General Overview
*			 For MBR:https://en.wikipedia.org/wiki/Master_boot_record , this is enough
*Note: This driver for now assumes the FAT drive to be partitioned with an MBR present starting at LBA=0
*Also Logical Block Adresses(LBA) are used. No support for CHS(cylinder head sector) type old files, nobody uses that anymore.
*This driver is specifically designed for the raspberry pi and provides no support for FAT16 or FAT8 type legacy file systems.
*
*NOTE: This FAT32 implementation assumes 1 cluster = 1 sector.
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

uint32_t previousRequestedCluster = 0xffffffff;
void readMBR() {
	emmcSendData(READ_SINGLE, 0, (uint32_t*)&masterBootRecord);
}

void readPartition1BPB() {
	uint8_t sdCardReadBuffer[512];
	printf("Partition LBA %x\n", masterBootRecord.partitionEntries[0].LBAOfFirstSector);
	emmcSendData(READ_SINGLE, masterBootRecord.partitionEntries[0].LBAOfFirstSector, (uint32_t*)&sdCardReadBuffer);
	// emmcSendData(READ_SINGLE, 0x2020, (uint32_t*)&sdCardReadBuffer2322);
	// for (uint8_t i=0; i < 128; i = i + 8) {
	// 	printf("%i: \t", i);
	// 	printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sdCardReadBuffer2322[i], sdCardReadBuffer2322[i+1], sdCardReadBuffer2322[i+2], sdCardReadBuffer2322[i+3], sdCardReadBuffer2322[i+4], sdCardReadBuffer2322[i+5], sdCardReadBuffer2322[i+6], sdCardReadBuffer2322[i+7]);
	// }
	
	// for(int i =0; i<90; i++) {
	// 	printf("%x ", sdCardReadBuffer[i]);
	// 	if(i%9 == 0) {
	// 		printf("\n");
	// 	}
	// }
	// my_memcpy(&partition1, &sdCardReadBuffer, 90); //0x5a
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

	printf("bytes per sector %x\n", partition1.BPB_ReservedSectorsCount);
	printf("oem name 0 %x\n", sdCardReadBuffer[3]);
	printf("oem name 1 %x\n", sdCardReadBuffer[4]);
}

void readRootDirectory() {
	/*
	*First calculate the number of clusters that this directory is spread into.
	*Create a buffer with all those sectors in it.
	*Then from that buffer, calculate the number of files/subdirectory entries in it. 
	* NOTE the VFAT long file names while counting the entries.
	*Create an array of the struct directoryEntry of the count of entries present.
	*Fill 'em up.
	*/

	//NOTE: ASSUMING 1 cluster = 1 sector

	uint16_t numberOfClustersOccupiedByDirectoryEntry = 1; 
	uint32_t currentCluster = 2; //hard coded 2 since its the root directory, will always be here											
	uint32_t nextClusterAsPerFAT = getNextClusterFromFAT(currentCluster); 
	currentCluster = nextClusterAsPerFAT;
	while(nextClusterAsPerFAT < 0x0ffffff8) {
		numberOfClustersOccupiedByDirectoryEntry += 1;
		nextClusterAsPerFAT = getNextClusterFromFAT(currentCluster);
		currentCluster = nextClusterAsPerFAT;
	}

	//This will be the array with all the root directory entries after it is filled up in a while
	uint8_t rootDirectoryBuffer[numberOfClustersOccupiedByDirectoryEntry * 512];



	//Lets fill in this buffer now.
	uint8_t sdCardReadBuffer[512];
	
	uint32_t rootDirectoryClusterNumber = masterBootRecord.partitionEntries[0].LBAOfFirstSector
											// + partition1.BPB_RootDirectoryCluster NOT SURE IF I HAVE TO ADD THIS OR NOT, COULDNT INTERPRET FROM THE DOCUMENTATION
											+ partition1.BPB_ReservedSectorsCount
											+ (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table);

	emmcSendData(READ_SINGLE, rootDirectoryClusterNumber, (uint32_t*)&sdCardReadBuffer);
	my_memcpy(rootDirectoryBuffer, (uint8_t*)&sdCardReadBuffer, 512, 0);
	currentCluster = 2; //hard coded 2 since its the root directory, will always be here											
	nextClusterAsPerFAT = getNextClusterFromFAT(currentCluster); 
	currentCluster = nextClusterAsPerFAT;
	uint16_t i = 1;
	while(nextClusterAsPerFAT < 0x0ffffff8) {
		uint32_t clusterToRead = nextClusterAsPerFAT - 2
								 + masterBootRecord.partitionEntries[0].LBAOfFirstSector
								 + partition1.BPB_ReservedSectorsCount
								 + (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table);
		
		emmcSendData(READ_SINGLE, clusterToRead, (uint32_t*)&sdCardReadBuffer);
		my_memcpy(rootDirectoryBuffer + i*512, (uint8_t*)&sdCardReadBuffer, 512, 0);
		i++;
		nextClusterAsPerFAT = getNextClusterFromFAT(currentCluster);
		currentCluster = nextClusterAsPerFAT;
	}	
	//rootDirectoryBuffer is filled at this point!!
	

	//Now we have to calculate the number of files/subdirectories in it while ignoring the LFN's.
	uint16_t numberOfEntries = 0;
	
	for(uint32_t i = 0; i < (uint32_t)((numberOfClustersOccupiedByDirectoryEntry*512) / 32); i++) {
		
		//Check for "end of directory entries marker" (i.e. first byte of entry is 0x00) and exit from loop
		if(rootDirectoryBuffer[i*32] == 0x00) {
			break;
		}

		//Count as an entry only if attribute says it is not a LFN entry (characterised by 0x0F)
		if(rootDirectoryBuffer[i*32 + 0x0B] != 0x0F) {
			numberOfEntries++;
		}
	}
	//Now we have numberOfEntries ready here to create an array of struct directoryEntries
	directoryEntry_t entries[numberOfEntries];

	//Lets fill those structs.
	uint16_t actualEntryNumber = 0;
	uint16_t k = 0;
	while(k < (numberOfClustersOccupiedByDirectoryEntry*512) / 32) {

		if(rootDirectoryBuffer[k*32] == 0x00) {
			break;
		}
		
		if(rootDirectoryBuffer[k*32 + 0x0B] == 0x0F) { //entry is LFN, fill name manually and other entries too
			uint8_t numberOfLFNEntries = 1;
			uint8_t sequenceNumber = rootDirectoryBuffer[k*32];
			if(sequenceNumber & 0x40) {
				numberOfLFNEntries = sequenceNumber & 0x0F;
			}
			for(uint8_t p = 0; p < numberOfLFNEntries; p++) {
				for(uint8_t t=0; t<10; t++) {
					entries[actualEntryNumber].name[p*26 + t] = rootDirectoryBuffer[(k+(numberOfLFNEntries - p - 1)) * 32 + 1 + t]; //+1 at rhs because first  entry is sequence number	
				}
				for(uint8_t t=0; t<12; t++) {
					entries[actualEntryNumber].name[p*26 + t + 10] = rootDirectoryBuffer[(k+(numberOfLFNEntries - p - 1)) * 32 + 0x0E + t]; //+1 at rhs because first  entry is sequence number	
				}
				for(uint8_t t=0; t<4; t++) {
					entries[actualEntryNumber].name[p*26 + t + 22] = rootDirectoryBuffer[(k+(numberOfLFNEntries - p - 1)) * 32 + 0x1C + t]; //+1 at rhs because first  entry is sequence number	
				}
			}
			k = k + numberOfLFNEntries;
		} else { //just directly copy 32 bytes to the struct taking care of the oversized name field in the struct buffed up to accomodate the LFN
			for(uint8_t a = 0; a<11; a++) {
				entries[actualEntryNumber].name[a] = rootDirectoryBuffer[k*32 + a];
			}
		}

		my_memcpy((void*)&entries[actualEntryNumber] + 26*3, (void*)&rootDirectoryBuffer, 21, (k*32) + 11);
		k += 1;
		actualEntryNumber += 1;
	}
	

	//print directory contents
	for(int v = 0; v<numberOfEntries; v++) {
		for(int nameCounter = 0; nameCounter<26; nameCounter++) {
			printf("%c", entries[v].name[nameCounter]);	
		}
		// printf("Attribute: %x       ", entries[k].attribute);
		printf("Size: %x       ", entries[v].size);
		/*********************************************************
		*REMEMBER CLUSTER NUMBERING BEGINS AT 2, so SUBTRACT 2 from LOW CLUSTER NUMBER
		*FOUND IN THE DIRECTORY ENTRY TO SEEK THE CONTENTS OF THE FILE.
		*/
		printf("FirstCluster low: %x       ",entries[v].firstClusterLow);
		printf("FirstCluster high: %x\n",entries[v].firstClusterHigh);
	}

	
}
	
uint32_t getNextClusterFromFAT(uint32_t currentClusterNumber) {
	uint8_t sdCardReadBuffer[512];
	uint32_t FATStartCluster = masterBootRecord.partitionEntries[0].LBAOfFirstSector
								+ partition1.BPB_ReservedSectorsCount;
	
	uint32_t clusterOffsetFromStartOfFAT = (uint32_t)(currentClusterNumber/128);
	uint32_t indexInCluster = currentClusterNumber%128;
	if (clusterOffsetFromStartOfFAT != (uint32_t)(previousRequestedCluster/128)) {
	//Load the part of the FAT table into fat32 array buffer as it is not present in the buffer
		//MEND THINGS HERE!!!!!!!
		emmcSendData(READ_SINGLE, FATStartCluster + clusterOffsetFromStartOfFAT, (uint32_t*) &sdCardReadBuffer);
		// my_memcpy(&fat32, &sdCardReadBuffer, )
		memcpy(&fat32, &sdCardReadBuffer, 512);
	}
	delay(1000);

	previousRequestedCluster = currentClusterNumber;
	return fat32[indexInCluster] & 0x0fffffff;

}
//ABI of passing struct with variable sized array member has changed	
uint8_t readFile(uint16_t fileFirstClusterLow, uint16_t fileFirstClusterHigh, uint32_t fileSize, uint8_t* file) {
	
	// uint8_t file[fileSize]; //fileSize stored in 32byte directory entries is in bytes, not sectors
	uint32_t fileFirstCluster = ((fileFirstClusterHigh<<16) | fileFirstClusterLow);	
	uint32_t fileFirstClusterToRead = masterBootRecord.partitionEntries[0].LBAOfFirstSector
								+ partition1.BPB_ReservedSectorsCount
								+ (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table) 
								+ fileFirstCluster -2;
	
	
	printf("FIRST cluster %x\n", fileFirstClusterToRead);
	uint8_t sdCardReadBuffer[512];

	emmcSendData(READ_SINGLE, fileFirstClusterToRead, (uint32_t*) &sdCardReadBuffer);
	memcpy(file, &sdCardReadBuffer, 512);

	// printSector(file);

	uint32_t nextClusterAsPerFAT = getNextClusterFromFAT(fileFirstCluster);
	printf("NExt %x\n", nextClusterAsPerFAT);
	printf("filefirstCluster %x\n", fileFirstCluster);
	uint32_t nextClusterToRead = 0;
	uint32_t previousCluster = 0xFFFFFFFF;
	uint32_t i = 1;
	while(nextClusterAsPerFAT<0x0ffffff8) {
		
		if(nextClusterAsPerFAT == 0x0ffffff7) {
			printf("\n****BAD CLUSTER ENCOUNTERED WHILE READING FILE****\n");
			break;
		}
		printf("NExt %x\n", nextClusterAsPerFAT);
		previousCluster = nextClusterAsPerFAT;
		nextClusterToRead = nextClusterAsPerFAT - 2 
							+ masterBootRecord.partitionEntries[0].LBAOfFirstSector
							+ partition1.BPB_ReservedSectorsCount
							+ (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table);
								//The -2 is mandatory, cluster numbering starts from 2
		emmcSendData(READ_SINGLE, nextClusterToRead, (uint32_t*) &sdCardReadBuffer);
		memcpy(file + (i*512), &sdCardReadBuffer, 512);

		nextClusterAsPerFAT = getNextClusterFromFAT(previousCluster);
		i += 1;
		if (i > 121) {
			break;
		}
	}

	// file_t fileStruct;
	// fileStruct.size = fileSize;
	// memcpy(fileStruct.file, file, (i-1)*512);

	//NOTE:This corrupts a portion of memory in the array "file" if the file size is not a multiple of 512.
	clearScreen();
	setStartPosition(0,0);
	setCursor(0);
	// for(uint32_t i = 0; i < fileSize; i++)
	// {
	// 	printSector(file + i);
	// }
	return 0;
}

void readDirectory(uint16_t directoryFirstClusterLow, uint16_t directoryFirstClusterHigh) {
	uint32_t directoryFirstCluster = ((directoryFirstClusterHigh<<16) | directoryFirstClusterLow);
	uint32_t directoryFirstClusterToRead = masterBootRecord.partitionEntries[0].LBAOfFirstSector
								+ partition1.BPB_ReservedSectorsCount
								+ (partition1.BPB_NumberOfFATs * partition1.BPB_SectorsPerFAT32Table) 
								+ directoryFirstCluster -2 ;
								//NOTE: -2 wont work for root directory, hence use different function for reading root directory	
	
	//First let's count the number of entries present here, so we can creat an array of directory entries
	uint16_t numberOfClustersOccupiedByDirectory = 1;
	uint32_t nextClusterAsPerFAT = getNextClusterFromFAT(directoryFirstCluster);
	uint32_t previousCluster = 0;
	while(nextClusterAsPerFAT < 0x0ffffff8) {
		previousCluster = nextClusterAsPerFAT;
		nextClusterAsPerFAT = getNextClusterFromFAT(previousCluster);
		numberOfClustersOccupiedByDirectory++;
	}

	//Create an array of directory entries of length 16*numberOfClustersOccupiedByDirectory 
	//as each cluster stores 16 entries
	directoryEntry_t entries[16*numberOfClustersOccupiedByDirectory];

	uint8_t sdCardReadBuffer[512];
	
	//Time to fill up these structs
	nextClusterAsPerFAT = getNextClusterFromFAT(directoryFirstCluster);
	previousCluster = 0;
	
	//load from first cluster
	emmcSendData(READ_SINGLE, directoryFirstClusterToRead, (uint32_t*)&sdCardReadBuffer);
	for(int k=0; k<16; k++) {
		//Maybe try using my_memcpy if this does not work
		memcpy(&entries[0], &sdCardReadBuffer[k*32] , 32); //!!!!entry[0] is the directory info
	}

	//load from consecutive clusters
	uint16_t i=1;
	while(nextClusterAsPerFAT < 0x0ffffff8) {
		previousCluster = nextClusterAsPerFAT;
		nextClusterAsPerFAT = getNextClusterFromFAT(previousCluster);
		emmcSendData(READ_SINGLE, directoryFirstClusterToRead, (uint32_t*)&sdCardReadBuffer);
		for(int k=0; k<16; k++) {
			memcpy(&entries[i*16 + k], &sdCardReadBuffer[k*32] , 32);
		}	
		i++;
	}

}


void my_memcpy(uint8_t* destination, uint8_t* source, uint32_t length, uint32_t sourceOffset) {
	for(uint32_t i=0; i<length; i++) {
		destination[i] = source[i+sourceOffset];
	}

}

void printSector(uint8_t* sector) {
	for (uint8_t i=0; i < 128; i = i + 8) {
		printf("%i: \t", i);
		printf("%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\t\t\t%x\n", sector[i], sector[i+1], sector[i+2], sector[i+3], sector[i+4], sector[i+5], sector[i+6], sector[i+7]);
	}
}

// void cat(char* fileName) {

// 	static volatile uint32_t currentDirectoryOld;
// 	static volatile uint8_t DirectoryBuffer[512];
// 	if (currentDirectoryOld != currentDirectory) {
// 		emmcSendData(READ_SINGLE, currentDirectory, (uint32_t*)&DirectoryBuffer);
// 		currentDirectoryOld = currentDirectory;
// 	}

	

// 	directoryEntry_t entries[15];
// 	for(int k=0; k<15; k++) {
// 		my_memcpy(&entries[k], &DirectoryBuffer, 32, (k+1)*32);



	
// }
