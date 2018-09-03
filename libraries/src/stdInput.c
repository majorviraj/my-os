#include <stdInput.h>
#include <stdOutput.h>

/*Use scanf function just like stdio scanf
 *REMEMBER TO CALL USBINITIALISE BEFORE USING THIS.
 */

void scanf(char *typeSpecifierString, char *inputBuffer)
{
	if (*typeSpecifierString == '%')
	{
		typeSpecifierString++;
		char charRead = 0;
		int enteredNumber;
		uint8_t noOfDigits;
		//The max lenght of the array
		int length_of_array = 0;
		if (*typeSpecifierString == 's')
		{
			printf("scanf() line: 23 - ERROR: Length of input not specified\n");
			return;
		}
		if (*typeSpecifierString >= '0' && *typeSpecifierString <= '9')
		{

			int j = 0;
			while (*(typeSpecifierString + j) != 's')
			{
				j++;
			}

			for (int i = 0; i < j; i++)
			{
				length_of_array = length_of_array * 10 + (*(typeSpecifierString + i) - '0');
			}

			typeSpecifierString += j;
		}

		switch (*typeSpecifierString)
		{
		case 's':
			while (charRead != '\n' && length_of_array > 0)
			{
				do
				{
					keyboardInit();
					charRead = KeyboardGetChar();
				} while (charRead == 0);

				printf("%c", charRead);

				if (charRead == '\n') {
					break;
				}
				*inputBuffer = charRead;
				inputBuffer++;
				length_of_array--;
			}

			break;
		case 'i':
			keyboardInit();
			charRead = KeyboardGetChar();
			uint8_t digit;
			uint8_t isNegative = 0;

			if (charRead == '-')
			{
				isNegative = 1;
				charRead = KeyboardGetChar();
			}

			while (charRead != '\n')
			{
				enteredNumber *= 10;
				charRead = KeyboardGetChar();
				digit = (int)(charRead - '0');
				enteredNumber += digit;
			}

			if (isNegative)
			{
				enteredNumber *= -1;
			}
			*inputBuffer = (char)enteredNumber;
			break;
		}
	}
}