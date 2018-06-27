/* c_hw_setup is called from cStartup.s after setting up interrupt vector table, clearing BSS
 * SP is setup, branch prediction is switched on, VFPU is enabled etc. 
 * Here in c_hw_setup mainly the MMU is enabled and kernel_main is called at the end.
 */

#include <intTypes.h>

#define MMU_PAGETABLE_BASE 	(64 << 14)
#define MMU_SECTION_1MB 	(1 << 4)
#define MMU_SECTION_16MB 	(2 << 4)
#define MMU_SUB_PAGES_64KB 	(2 << 0)
#define MMU_SUB_PAGES_16KB 	(1 << 0)

extern void _init_mmu(void *TLB_base, uint32_t);
extern void kernel_main(void);
void _mmu_addSectionEntryToPageTable(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t accessMode);

//For read-write access in both modes use APX=b0 AP[0:1]=b11
#define READ_WRITE_FOR_ALL 	(3 << 10) & (~(1 << 15))

//TO-DO : ADD MACROS FOR OTHER ACCESS MODES HERE.

int c_hw_setup()
{

	// if (page_table_code == MMU_SECTION_1MB)
	// {

		/* THIS FOR LOOP WILL MAP ALL 1MB SECTIONS FROM SAME VIRTUAL TO PHYSICAL ADDRESS.
		 * EXCEPT FOR MEMORY SECTIONS 16MB & 17MB WHICH ARE SWAP MAPPED.
		 */
		for (uint32_t i = 0; i < 0xFFF00000; i += 0x100000)
		{
			if (i == 0x1000000)
			{
				_mmu_addSectionEntryToPageTable(0x1000000, 0x1100000, READ_WRITE_FOR_ALL);
			} else if(i == 0x1100000) {
				_mmu_addSectionEntryToPageTable(0x1100000, 0x1000000, READ_WRITE_FOR_ALL);
			} else {
				_mmu_addSectionEntryToPageTable(i, i, READ_WRITE_FOR_ALL);
			}
		}
	// } else {
	// 	printf("c_hw_setup(): Other modes are not define yet");
	// }

	// NOTE: Only call this fuction when pagetable is properly filled
	*(volatile uint32_t*)0x1100001 = 0xDEADBEEF;
	*(volatile uint32_t*)0x1100002 = 0xCAFEBABE;
	_init_mmu(MMU_PAGETABLE_BASE, 0x00000001|0x1000|0x0004);

	// WARNING: Only call kernel main after all setup is done
	kernel_main();
	return 0;
}

void _mmu_addSectionEntryToPageTable(uint32_t virtualAddress, uint32_t physicalAddress, uint32_t accessMode)
{
	uint32_t offsetInSectionDescriptorTable = virtualAddress >> 20;
	uint32_t sectionDescriptorAddress = (offsetInSectionDescriptorTable << 2) | MMU_PAGETABLE_BASE & (~0b11); //~0b11 to ensure LSB 2 bits are both 0.
	*(uint8_t *)sectionDescriptorAddress = (physicalAddress & 0xFFF00000) | accessMode | 0b10;
	//0b10 indicates it is a section entry and no 2nd level coarse page table is used.
}
