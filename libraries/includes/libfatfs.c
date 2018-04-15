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
	uint8_t mbrBuffer[512];
	mbrBuffer[511] = 0x52;
	masterBootRecord_t* masterBootRecord = (masterBootRecord_t*)&mbrBuffer;
	printf("new mbr sign %x\n", masterBootRecord->MBR_bootSignature);
	emmcSendData(READ_SINGLE, 0, (uint32_t*)&mbrBuffer);
	printf("mbrSign %x\n", mbrBuffer[510]);
	masterBootRecord = (masterBootRecord_t*)&mbrBuffer;
	printf("mbrSign %x\n", mbrBuffer[0x1c2]);
	printf("mbrSign %x\n", mbrBuffer[511]);
	printf("mbrSign %x\n", masterBootRecord[511]);
}