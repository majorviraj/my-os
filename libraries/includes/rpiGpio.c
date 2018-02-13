#include "rpiGpio.h"

unsigned int stateOfLed=0;

void gpioToggle(unsigned int gpioNumber) {
	stateOfLed = !stateOfLed;
	if (stateOfLed==1) {
		gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);
	} else if(stateOfLed==0) {
		gpio[LED_GPSET] = (1 << LED_GPIO_BIT);
	} else{}
}