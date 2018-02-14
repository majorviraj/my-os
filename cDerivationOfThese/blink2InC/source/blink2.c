#include <includes/rpiGpio.h>

volatile unsigned int* gpio = (unsigned int*)PERIPHERAL_BASE;

volatile unsigned int tim;
void kernel_main() {
    // gpio = (unsigned int*) GPIO_BASE;

    gpio[LED_GPFSEL] |= (1 << LED_GPIO_BIT);

    while(1) {

        for (tim = 0; tim < 500000; tim++);

        gpio[LED_GPCLR] = (1 << LED_GPIO_BIT);

        for (tim = 0; tim < 500000; tim++);
        
        gpio[LED_GPSET] = (1 << LED_GPIO_BIT);
    }
}