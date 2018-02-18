#ifndef _ASSEMBLY_FUNCTIONS_
#define _ASSEMBLY_FUNCTIONS_

extern int frameBufferInit(int, int, int);
extern void UsbInitialise();
extern void keyboardInit();
extern char KeyboardGetChar();
extern int drawCharacter(int, int, int);
extern void _enable_interrupts();

#endif