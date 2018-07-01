#include <rpiGpio.h>
#include <assemblyFunctions.h>

void kernel_main() {

	gpio[16] |= (1 << 21);
	while(1) {

		delayMicro(400 * 1000);
		gpio[32] = (1 << 15);

		delayMicro(400 * 1000);
		gpio[44] = (1 << 15);
    	}
}
