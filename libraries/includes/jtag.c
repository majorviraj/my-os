#include <rpiGpio.h>

void jtagInit()  {
	volatile unsigned int* gpio = returnGpio();

	gpio[GPIO_GPPUD] = 0;
	for (volatile int i = 0; i<150; i++);
	gpio[GPIO_GPPUDCLK0] = (1<<4)|(1<<22)|(1<<24)|(1<<25)|(1<<27);
	for (volatile int i = 0; i<150; i++);
	gpio[GPIO_GPPUDCLK0] = 0;


	gpio[GPIO_GPFSEL0] &= ~(7<<12);
	gpio[GPIO_GPFSEL0] |= (2<<12);

    gpio[GPIO_GPFSEL2] &= ~(7<<6);	//gpio22
    gpio[GPIO_GPFSEL2] |= 3<<6;		//alt4 ARM_TRST
    gpio[GPIO_GPFSEL2] &= ~(7<<12); //gpio24
    gpio[GPIO_GPFSEL2] |= 3<<12;	//alt4 ARM_TDO
    gpio[GPIO_GPFSEL2] &= ~(7<<15); //gpio25
    gpio[GPIO_GPFSEL2] |= 3<<15;	//alt4 ARM_TCK
    gpio[GPIO_GPFSEL2] &= ~(7<<21); //gpio27
    gpio[GPIO_GPFSEL2] |= 3<<21;	//alt4 ARM_TMS

}
