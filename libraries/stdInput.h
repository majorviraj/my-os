#ifndef STD_INPUT
#define STD_INPUT

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>

//Scanf function reads a string of characters from the keyboard and keeps polling until the 
//"enter" escape sequence is received.
void scanf(char *, char *); 

#endif	/*STD_INPUT */