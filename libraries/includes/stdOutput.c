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

void put(char character, uint32_t *cursor) {
	drawCharacter(character, startX + (*cursor*8)%1358, 
					(uint32_t)(16*((*cursor*8)/1358)) + startY);
	*cursor++;
}


void putChar(char character) {
	put(character, &cursorPosition);
}


void putInt(int x) {
	if (x<0) {
		put('-', &cursorPosition);
		x *= -1;	
	}
	
	while(x/10 != 0) {
		put(x%10 + 30, &cursorPosition);
		x = (int)x/10;
	}
	put(x, &cursorPosition);

}


void putString(char *string) {
	int i = 0;
	while(*(string + i) != '\0') {
		put(*(string+i), &cursorPosition);
		i++;
	}
}


void printf(char *string, ...) {
	va_list argumentsList;
	va_start(argumentsList, string);
	char *traverseString;
	traverseString = string;
	uint32_t i;
	char *s;
	for (traverseString = string; *traverseString!='\0'; traverseString++) {
		
		if(*traverseString == '%') {
			
			traverseString++;
			switch(*traverseString) {
				case 'c':
					i = va_arg(argumentsList, int);
					putChar(i);
					break;
				case 'i':
					i = va_arg(argumentsList, int);
					putInt(i);
				case 's':
					s = va_arg(argumentsList, char *);
					putString(s);
					break;
			}

		} else {
			putChar(*traverseString);
		}

	}
	va_end(argumentsList);
}
