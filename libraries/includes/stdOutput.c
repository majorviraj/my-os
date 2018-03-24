#include <stdOutput.h>

uint32_t cursorPosition =0;
uint32_t startX=0;
uint32_t startY=0;


void setStartPosition(unsigned int x, unsigned int y) {
	startX = x;
	startY = y;
	cursorPosition = 0;
}


void setCursor(unsigned int cursorPos) {
	cursorPosition = cursorPos;
}


//1366*78 -> 1358*62

void putChar(char character, uint32_t *cursor) {
	drawCharacter(character, startX + ((*cursor)*8)%1016, 
					(uint32_t)(16*(((*cursor)*8)/1016)) + startY);
	(*cursor)++;
}

//Checks if monitor is connected and calls putChar to print to display
//or writes to Serial console.
void put(char character) {
	putChar(character, &cursorPosition);
}



void putInt(int x) {
	if (x<0) {
		put('-');
		x *= -1;	
	}
	uint32_t lengthOfX = 1;
	int orignalX = x;
	
	while(x/10 > 10){
		lengthOfX *= 10;
		x = x/10;
	}
	x = orignalX;

	if (lengthOfX == 100000){
		gpioBlink(50, 20);
	}
	
	while(x > 0) {
		put((uint32_t)(x/lengthOfX) + 0x30);
		x -= (x/lengthOfX)*lengthOfX;
		lengthOfX = lengthOfX / 10;
	}
}


void putString(char *string) {
	int i = 0;
	while(*(string + i) != '\0') {
		put(*(string+i));
		i++;
	}
}


void printf(char *string, ...) {
	va_list argumentsList;
	va_start(argumentsList, string);
	char *traverseString;
	traverseString = string;
	int i;
	char *s;
	int c;
	for (traverseString = string; (*traverseString)!='\0'; traverseString++) {
		
		if(*traverseString == '%') {
			
			traverseString++;
			switch(*traverseString) {
				case 'c':
					i = va_arg(argumentsList, int);
					put(i);
					break;
				case 'i':
					i = va_arg(argumentsList, int);
					putInt(i);
					break;
				case 's':
					s = va_arg(argumentsList, char *);
					putString(s);
					break;
			}

		}
		else if (*traverseString == '\n') {
			startX = 0;
			startY += 16;
			setCursor(1);
		}

		else {
			put(*traverseString);
		}

	}
	va_end(argumentsList);
}









// void miniUARTInit(uint32_t baudRate, uint32_t frameSize) {
// 	ARMAux->auxEnable |= AUX_MINI_UART_ENABLE; //enable the MiniUART, cant access other registers before enabling.

// 	//disable interrupts
// 	ARMAux->auxMUIntEnable = 0;

// 	//Disable all extra features of MU ExtraControl register for CTS, RTS pins
// 	//Also disable transmitter and receiver b resetting pin 0 and pin 1 in the register.
// 	ARMAux->auxMUExtraControl = 0;

// 	//Configure frame size according to argument
// 	if (frameSize == 8) {
// 		ARMAux->auxMULineControl = 3<<0; //used because of Valver, says he saw it on errata info
// 	} else {
// 		ARMAux->auxMULineControl = 0;
// 	}

// 	//Disable all interrupts and clear the FIFOS
// 	ARMAux->auxMUIntEnable = 0;
// 	ARMAux->auxMUIntIdentify = 0xC6; //BECAUSE VALVERS DID IT.

// 	//Set Baud rate
// 	ARMAux->auxMUBaudrate = (SYSTEM_FREQUENCY/(8*baudRate))-1;

// 	//Setup GPIO 14 and 15 to alternate function 5.
// 	gpioSetFunction(14, ALT_FUNCTION5);
// 	gpioSetFunction(15, ALT_FUNCTION5);

// 	//Might have to set up pullup pulldown and pupdclk of gpio



// 	//Set tx bit in extra control lines register
// 	ARMAux->auxMUExtraControl = (1<<1);


// }

// void miniUARTWrite(char c) {
// 	while(ARMAux->auxMULineStatus & (1<<8) == 0 ) {
// 		//halt until previous buffer is transmitted
// 	}

// 	//Write character to be written in IO data register
// 	ARMAux->auxMUIO = c;
// }
