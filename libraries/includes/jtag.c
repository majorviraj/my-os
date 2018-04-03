#include <rpiGpio.h>
#include <assemblyFunctions.h>
void jtagInit()  {
	volatile unsigned int* gpio = returnGpio();

	gpio[GPIO_GPPUD] = 0;
	delayMicro(2);
	gpio[GPIO_GPPUDCLK0] = (1<<4)|(1<<22)|(1<<24)|(1<<25)|(1<<27);
	delayMicro(2);
	gpio[GPIO_GPPUDCLK0] = 0;

	
	gpio[GPIO_GPFSEL0] &= ~(7<<12);	// GPIO 4
	gpio[GPIO_GPFSEL0] |= (2<<12);	// Alternate Function 5 TDI
    gpio[GPIO_GPFSEL2] &= ~(7<<6);	// GPIO 22
    gpio[GPIO_GPFSEL2] |= 3<<6;		// Alternate function 4 TRST
    gpio[GPIO_GPFSEL2] &= ~(7<<12); // GPIO 24
    gpio[GPIO_GPFSEL2] |= 3<<12;	// Alternate function 4 TDO
    gpio[GPIO_GPFSEL2] &= ~(7<<15); // GPIO 25
    gpio[GPIO_GPFSEL2] |= 3<<15;	// Alternate function 4 TCK
    gpio[GPIO_GPFSEL2] &= ~(7<<21); // GPIO 27
    gpio[GPIO_GPFSEL2] |= 3<<21;	// Alternate function 4 TMS

}
