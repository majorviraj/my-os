#ifndef __ASSEMBLEY_FUNCTIONS_H_
#define __ASSEMBLEY_FUNCTIONS_H_

#include <intTypes.h>
extern void setForeColour(uint16_t);
extern void FullScreenToForeColour();
extern int frameBufferInit(int, int, int);
extern void UsbInitialise();
extern void keyboardInit();
extern char KeyboardGetChar();
extern int drawCharacter(int, int, int);	// char, x, y
extern void drawCircle(int, int, int);
extern void _enable_interrupts();

// extern void mailboxRead(uint32_t);
// extern void mailboxWrite(uint32_t);

// Functions defined in systemTimer.s

// Return the base of Timer Peripheral
extern uint32_t GetTimerBase();

// Return the value of timer right now
// extern unsigned int long GetTimeStamp();

// Halts the processor for the given amount of time
extern void delayMicro(uint32_t);

// Halts the processor for the given number of clock cycles
extern void delayCycles(uint32_t);

#endif