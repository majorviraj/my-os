#include <stdOutput.h>
#include <stdInput.h>
#include <uart.h>

int hh;
void frameBufferSetup(int width, int height, int bitDepth)
{
    int error = frameBufferInit(width, height, bitDepth);

    if (error == 0)
    {
        return 0;
    }

    else
    {
        // error handler
    }
}


void kernel_main()
{
	char* test = "yoko\nooyyoyo\tyooyoy";


	frameBufferSetup(1920, 1080, 16);
	printf(test);
	char n = '\n';
	printf("%c", n);
	int i = 0;
	printf("Width, %i\n", frameBufferData);
	
    while(1) {
		uart_putchar('Q');
        uart_putchar('X');
	}
}