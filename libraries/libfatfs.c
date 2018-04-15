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

masterBootRecord_t* masterBootRecord;
biosPartitionBlock_t* partition1;

void readMBR() {
	uint32_t mbrBuffer[128];
	emmcSendData(READ_SINGLE, 0, &mbrBuffer);
	masterBootRecord = mbrBuffer;

}