#include <rpiGpio.h>
#include <intTypes.h>
#include <timer.h>
#include <stdOutput.h>
volatile uint32_t* gpio = (unsigned int*)GPIO_BASE;
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
	

	// gpio[GPIO_GPPUD] = 2;
	// delayCycles(75);
	// gpio[GPIO_GPPUDCLK1] = (1 << 3);
	// delayCycles(75);
	// gpio[GPIO_GPPUD] = 0;
	// gpio[GPIO_GPPUDCLK1] = 0;

	gpio[LED_GPFSEL] &= ~(7 << 15);
	gpio[LED_GPFSEL] |= (1 << 15);
}

unsigned int* returnGpio() {
	return gpio;
}

void gpioBlink(uint16_t time,uint16_t number) {
	printf("in a blink time : %i, number : %i\n", time, number);
	// for (uint16_t i=0; i<number; i++) {
	// 	gpioToggle();
	// 	delay(time);
	// }
	// gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);
}
