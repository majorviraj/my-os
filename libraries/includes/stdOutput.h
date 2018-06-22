#ifndef _STD_OUTPUT_
#define _STD_OUTPUT_

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>

/*Just a wrapper for drawChar which takes input as a single character,
* either single ASCII character or single digit number, based on 4th parameter,
* which is a type specifier and can take values 'i' or 'c'
*/


void setCursor(unsigned int);

void setStartPosition(unsigned int x, unsigned int y);

//This is just a wrapper for 
void putChar(char character, uint32_t *cursor);

void put(char character);

void putInt(int x);

void putHex(uint32_t x);

void putString(char * string);

void printf(char *string, ...);

void clearScreen();


/*Code for enabling the MiniUART for debugging purposes. 
* Uses the pin8 (GPIO 14) as the Tx pin for UART.
* Uses the pin10 (GPIO 15) as the Rx pin for UART. 
* Call MiniUartInit function first to initialise the UART
* on GPIO 14 and 15 and use them for alternate function, AUX peripheral.
*/

#define AUX_BASE    ( PERIPHERAL_BASE + 0x215000 )

#define AUX_MINI_UART_ENABLE 	(1<<0)
#define AUX_ENABLE_SPI0 		(1<<1)
#define AUX_ENABLE_SPI1 		(1<<2)


#define SYSTEM_FREQUENCY    250000000



void setCursor(unsigned int);

void setStartPosition(unsigned int x, unsigned int y);

//This is just a wrapper for 
void putChar(char character, uint32_t *cursor);

void put(char character);

void putInt(int x);

void putString(char * string);

void printf(char *string, ...);

//Aux Mini UART Code

typedef struct {

	volatile uint32_t auxIRQ; //Check if any auxiliary (MiniUART, SPI0, SPI1) has pending IRQ
	volatile uint32_t auxEnable; //Enable MiniUART, SPI0, SPI1 here. REMEMBER TO FIRST SET ALTERNATE FUNCTION OF GPIO
	
	volatile unsigned int auxReserved1[((0x40 - 0x04) / 4) - 1]; //Reserved locations

	volatile uint32_t auxMUIO; //Mini Uart I/O Data
	volatile uint32_t auxMUIntEnable; //Mini UART interrupt enable
	volatile uint32_t auxMUIntIdentify; //Mini UART interrupt identify
	volatile uint32_t auxMULineControl; //To control unnecessary Data ready. RTS etc signals. Also set frame size in this
	volatile uint32_t auxMUModemControl; //Used to control the RTS line
	volatile uint32_t auxMULineStatus;//Used to check the status of the data buffers, IMPORTANT
	volatile uint32_t auxMUModemStatus; //Check the status of the model lines
	volatile uint32_t auxMUScratch; 
	volatile uint32_t auxMUExtraControl;
	volatile uint32_t auxMUExtraStatus;
	volatile uint32_t auxMUBaudrate; //the baudrate

	volatile unsigned int auxReserved2[(0x80 - 0x68) / 4];

	volatile uint32_t auxSPI0Control0;
	volatile uint32_t auxSPI0Control1;
	volatile uint32_t auxSPI0Status;
	volatile uint32_t auxSPI0Data;
	volatile uint32_t auxSPI0Peek;

	volatile unsigned int auxReserved3[(0xC0 - 0x94) / 4];

	volatile uint32_t auxSPI1Control0;
	volatile uint32_t auxSPI1Control1;
	volatile uint32_t auxSPI1Status;
	volatile uint32_t auxSPI1Data;
	volatile uint32_t auxSPI1Peek;

} auxStruct;


static auxStruct* ARMAux = (auxStruct*) AUX_BASE;

void miniUARTInit(uint32_t baudRate, uint32_t frameSize);

void miniUARTWrite(char c);


#endif