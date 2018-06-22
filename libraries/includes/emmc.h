#ifndef __EMMC_H__
#define __EMMC_H__

#include <rpiGpio.h>
#include <intTypes.h>
// Addres for the controller
// In case of dought do check this first of all
#define EMMC_CONTROLLER (PERIPHERAL_BASE + 0x300000)

// Commmand flags
#define CMD_TYPE_NORMAL  0x00000000
#define CMD_TYPE_SUSPEND 0x00400000
#define CMD_TYPE_RESUME  0x00800000
#define CMD_TYPE_ABORT   0x00c00000
#define CMD_IS_DATA      0x00200000
#define CMD_IXCHK_EN     0x00100000
#define CMD_CRCCHK_EN    0x00080000
#define CMD_RSPNS_NO     0x00000000
#define CMD_RSPNS_136    0x00010000
#define CMD_RSPNS_48     0x00020000
#define CMD_RSPNS_48B    0x00030000
#define INT_READ_RDY     0x00000020
#define INT_WRITE_RDY    0x00000010
#define TM_MULTI_BLOCK   0x00000020
#define TM_DAT_DIR_HC    0x00000000
#define TM_DAT_DIR_CH    0x00000010
#define TM_AUTO_CMD23    0x00000008
#define TM_AUTO_CMD12    0x00000004
#define TM_BLKCNT_EN     0x00000002
#define TM_MULTI_DATA (CMD_IS_DATA|TM_MULTI_BLOCK|TM_BLKCNT_EN)

#define A_COMMAND_ID 		0x80000000

// CMDTM register values for all commands
#define GO_IDLE_STATE (0 << 24)|CMD_RSPNS_NO
#define ALL_SEND_CID (2 << 24)|CMD_RSPNS_136
#define SEND_RELATIVE_ADDR (3 << 24)|CMD_RSPNS_48
#define CARD_SELECT (7 << 24)|CMD_RSPNS_48B
#define SEND_IF_COMMAND (8 << 24)|CMD_RSPNS_48
#define READ_SINGLE (17 << 24)|CMD_RSPNS_48|CMD_IS_DATA|TM_DAT_DIR_CH
#define READ_MULTI (18 << 24)|CMD_RSPNS_48|TM_MULTI_BLOCK|TM_DAT_DIR_CH
#define WRITE_SINGLE (24 << 24)|CMD_RSPNS_48|CMD_IS_DATA|TM_DAT_DIR_HC
#define WRITE_MULTI (25 << 24)|CMD_RSPNS_48|TM_MULTI_BLOCK|TM_DAT_DIR_HC 
#define APP_CMD_NO_RCA (55 << 24)|CMD_RSPNS_NO
#define APP_CMD (55 << 24)|CMD_RSPNS_48

#define SET_BUS_WIDTH (6 << 24)|CMD_RSPNS_48|A_COMMAND_ID
#define SEND_SCR (51 << 24)|CMD_RSPNS_48|CMD_IS_DATA|TM_DAT_DIR_CH| A_COMMAND_ID
#define SD_OP_COND (41 << 24)|CMD_RSPNS_48|A_COMMAND_ID

typedef struct {
	// Register for the argument 2 of command
    volatile uint32_t arg2;

	// Size and numberof blocks to read/write to the sd card
	volatile uint32_t blockSizeCount;

	// Register for the argument 1 of command
	volatile uint32_t arg1;

	// Command and responce reqister
	volatile uint32_t cmdtm;

	// Responce from card in 4 registers 1,2,3 and 4
	volatile uint32_t responce0;	
	volatile uint32_t responce1;
	volatile uint32_t responce2;
	volatile uint32_t responce3;

	// Data returned by the register
	volatile uint32_t data;

	// Status of the current tranfer
	volatile uint32_t status;
	
	// Configuration bits for Host
	volatile uint32_t control0;
	volatile uint32_t control1;

	// Interrupt Flags
	volatile uint32_t interrupt;

	// Interrupt Flags enable
	volatile uint32_t interruptMask;

	// Interrupt Generation Enable
	volatile uint32_t interruptEnable;
	volatile uint32_t control2;
	
} emmcControllerBasicStruct1;

static emmcControllerBasicStruct1* emmcControllerBasicStruct1_t = (emmcControllerBasicStruct1*) EMMC_CONTROLLER;
volatile uint32_t* emmcControllerBasicForceInterrupt;

typedef struct {

	// Timeout in boot mode
	volatile uint32_t bootTimeout;

	// Debug Bus Configuration
	volatile uint32_t debugSelect;

	// DO NOT USE RESERVED REGISTERS
	volatile uint32_t reserved0;
	volatile uint32_t reserved1;

	// Extention FIFO Registers
	volatile uint32_t extFIFOConfig;
	volatile uint32_t extFIFOEnable;

	// Delay per card clock tuning step
	volatile uint32_t tuneStep;

	// Card clock tuning steps for SDR
	volatile uint32_t tuneStepsSTD;

	// Card clock tuning steps for DDR
	volatile uint32_t tuneStepsDDR;
} emmcControllerBasicStruct2;

static emmcControllerBasicStruct2* emmcControllerBasicStruct2_t = (emmcControllerBasicStruct2*) EMMC_CONTROLLER;

volatile uint32_t* spiInterruptSupport;
volatile uint32_t* slotInterruptAndVersion;


struct emmcDevice {
	uint32_t cardOCR;
	uint32_t cardSupports18v;
	uint32_t cardIsSDHC;
	uint32_t cardRCA;
	uint32_t cardBusWidth;

};
void emmcGetStatus();

void emmcAllRegisters();

uint32_t emmcSendCommand(uint32_t, uint32_t, uint32_t);

void emmcInit();
void emmcSendData(uint32_t, uint32_t, uint32_t*);

#endif
