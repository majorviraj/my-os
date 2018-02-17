#include <rpiGpio.h>

volatile unsigned int* gpio = (unsigned int*)GPIO_BASE;
unsigned int stateOfLed=0;

void gpioToggle() {
	
	stateOfLed = !stateOfLed;
	if (stateOfLed==1) {
		gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);
	
	} else if(stateOfLed==0) {
		gpio[LED_GPSET] = (1 << LED_GPIO_BIT);
	}

}

void setLEDasOutput() {
	gpio[LED_GPFSEL] |= (1 << LED_GPIO_BIT);
}
