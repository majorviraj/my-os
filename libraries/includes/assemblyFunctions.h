#ifndef __ASSEMBLEY_FUNCTIONS_H_
#define __ASSEMBLEY_FUNCTIONS_H_

#include <intTypes.h>

extern int frameBufferInit(int, int, int);
extern void UsbInitialise();
extern void keyboardInit();
extern char KeyboardGetChar();
extern int drawCharacter(int, int, int);
extern void _enable_interrupts();

// Functions defined in systemTimer.s

// Return the base of Timer Peripheral
extern uint32_t GetTimerBase();

// Return the value of timer right now
// extern unsigned int long GetTimeStamp();

// Halts the processor for the given amount of time
extern void delayMicro(uint32_t);

#endif