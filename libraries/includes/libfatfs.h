#ifndef LIB_FAT_FS
#define LIB_FAT_FS

#include <intTypes.h>

//Partition entry in the MBR, total size = 16 Bytes
typedef struct partitionEntry_struct {
	uint8_t statusBootable;
	uint8_t CHSAddressOfFirstSector[3];
	uint8_t partitionType;
	uint8_t CHSAddressOfLastSector[3];
	uint32_t LBAOfFirstSector; //important field
	uint32_t numberOfSectorsInPartition;
} __attribute__((packed)) partitionEntry_t;

//MBR
typedef struct masterBootRecord_struct {
	uint8_t MBR_reservedCodeArea1[218];
	uint8_t MBR_diskTimeStamp[6];
	uint8_t MBR_reservedCodeArea2[216];
	uint8_t MBR_diskSignature[6];
	partitionEntry_t partitionEntries[4];
	uint16_t MBR_bootSignature;
} masterBootRecord_t;

//Boot Entry, first sector of any partition
typedef struct bootEntry_struct {
	uint8_t BS_jmpBoot[3];				// 0x000 ;jump for legacy reasons
	uint8_t BS_OEMName[8]; 				// 0x003 ;OEM Name in ASCII 
	uint16_t BPB_BytesPerSector; 		// 0x00b ;Bytes per sector. Allowed values include 512,1024, 2048, and 4096 
	uint8_t BPB_SectorsPerCluster; 		// 0x00d ;Sectors per cluster (data unit). Allowed values are powers of 2, but the cluster size must be 32KB  or smaller 
	uint16_t BPB_ReservedSectorsCount;	// 0x00e ;Size in sectors of the reserved area 
	uint8_t BPB_NumberOfFATs; 			// 0x010 ;Number of FATs 
	uint16_t BPB_RootEntryCount;		// 0x011 ;Maximum number of files in the root directory for  FAT12 and FAT16. This is 0 for FAT32 
	uint16_t BPB_TotalSectorsFAT16; 	// 0x013;16-bit value of number of sectors in file system 
	uint8_t BPB_MediaType; 				// 0x015 ;Media type 
	uint16_t BPB_FATSize16;				// 0x016 ;For FAT12/FAT16 only, 0 for FAT32, see offset 0x024
	uint16_t BPB_SectorsPerTrack; 		// 0x018 ;Sectors per track of storage device 
	uint16_t BPB_NumberOfHeads;			// 0x01a ;Number of heads in storage device 
	uint32_t BPB_HiddenSectors;			// 0x01c ;Number of sectors before the start of partition 
	uint32_t BPB_TotalSectors32;		// 0x020 ;32-bit value of number of sectors in file system.  Either this value or the 16-bit value above must be  0 
	uint32_t BPB_SectorsPerFAT32Table;	// 0x024 ;32-bit size in sectors of one FAT table
	uint16_t BPB_ExtFlags;				// 0x028 ;A flag for FAT 
	uint16_t BPB_FSVersion;				// 0x02a ;The major and minor version number 
	uint32_t BPB_RootDirectoryCluster;	// 0x02c ;Cluster where the root directory can be found 
	uint16_t BPB_FSInfo;				// 0x030 ;Sector where FSINFO structure can be found
	uint16_t BPB_BackupBootSec;			// 0x032 ;Sector where backup copy of boot sector is located 
	uint8_t BPB_Reserved[12];			// 0x034 ;Reserved 
	uint8_t BS_DrvNum;					// 0x040 ;BIOS INT13h drive number 
	uint8_t BS_Reserved1;				// 0x041 ;Not used 
	uint8_t BS_BootSig;					// 0x042 ;Extended boot signature to identify if the next three values are valid 
	uint32_t BS_VolID;					// 0x043 ;Volume serial number 
	uint8_t BS_VolLab[11];				// 0x047 ;Volume label in ASCII. User defines when creating the file system 
	uint8_t BS_FileSystemType[8];		// 0x052 ;File system type label in ASCII 
} biosParameterBlock_t;



typedef struct directoryEntry_struct {
	uint8_t name[11];				// Offset; 0x00 - (11 bytes)	
	uint8_t attribute; 				// Offset: 0x0B
	uint8_t reserved; 				// Reserved for Windows NT (Supposedly this tells us about the casing)
    uint8_t creationTimeInTenths; 	// Creation time in tenths of a second. Note: Only 24 bits used
    uint16_t createTime; 			// 5 bits Hour, 6 bits minutes, 5 bits seconds
    uint16_t createDate; 			// 7 bits year, 4 bits month, 5 bits day
    uint16_t lastAccessDate; 		// 
    uint16_t firstClusterHigh; 		// High 16 bits is first cluster number
    uint16_t lastModifiedTime;		//
    uint16_t lastModifiedDate;		//
    uint16_t firstClusterLow;  		// Low 16 bits is the first cluster number
    uint32_t size; 					// In bytes
    uint8_t hasLongName;
    uint8_t longName[256];
} directoryEntry_t;


masterBootRecord_t masterBootRecord;
biosParameterBlock_t partition1;
directoryEntry_t rootDirectory;

void readMBR();
void readPartition1BPB();
void readRootDirectory();
void my_memcpy(uint8_t*, uint8_t*, uint32_t, uint32_t);
#endif