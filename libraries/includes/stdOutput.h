#ifndef _STD_OUTPUT_
#define _STD_OUTPUT_

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>

/*Just a wrapper for drawChar which takes input as a single character,
* either single ASCII character or single digit number, based on 4th parameter,
* which is a type specifier and can take values 'i' or 'c'
*/


void setCursor(unsigned int);

void setStartPosition(unsigned int x, unsigned int y);

//This is just a wrapper for 
void put(char character, uint32_t *cursor);

void putChar(char character);

void putInt(int x);

void putString(char * string);

void printf(char *string, ...);

#endif