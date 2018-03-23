#include <stdInput.h>


/*Use scanf function just like stdio scanf
*REMEMBER TO CALL USBINITIALISE BEFORE USING THIS.
*/


void scanf(char *typeSpecifierString, char* inputBuffer) {
	keyboardInit();
	while(*typeSpecifierString != '\0') {
		

		if(*typeSpecifierString == '%') {
			typeSpecifierString++;
			char charRead;
			int enteredNumber;
			uint8_t noOfDigits;
			switch(*typeSpecifierString) {
				case 's':
					charRead = KeyboardGetChar();
					put(charRead);
					while (charRead != '\n') {
						*inputBuffer = charRead;
						charRead = KeyboardGetChar();
						inputBuffer++;
					}	
					break;
				case 'i':
					charRead = KeyboardGetChar();
					uint8_t digit;
					uint8_t isNegative = 0;
					
					if(charRead == '-') {
						isNegative = 1;
						charRead = KeyboardGetChar();
					}
				
					while(charRead != '\n') {
						enteredNumber *= 10;
						charRead = KeyboardGetChar();
						digit = (int)(charRead - '0');
						enteredNumber += digit;

					}

					if(isNegative) {
						enteredNumber *= -1;
					}
					*inputBuffer = (char *)enteredNumber; 
					break;
					
			}

		} else {
			put(*typeSpecifierString);
			typeSpecifierString++;
		}


	}


	


	 





	return 0;
}