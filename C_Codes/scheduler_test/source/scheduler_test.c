#include <stdOutput.h>
#include <stdInput.h>
#include <scheduler.h>
#include <lib_uart.h>
#include <timer.h>

uint8_t x = 0;

uint32_t newCursor = 30;

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


void task2() {
	while(1) {
		// uart_putchar("2");
		x = 4;
	}
}

void task3 () {
	while (1)
	{
		// if (x !=0)
		// 	uart_putchar(x);

		x = 5;
	}
}


void kernel_main()
{
	char *test = "yoko\nooyyoyo\tyooyoy";
	frameBufferSetup(1024, 768, 16);
	// uart_init();
	//timerInit(2 * LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

	//printf("%x\t", );
	printf("Begin: asd\n");

	// uart_putchar('B');
	// uart_putchar('E');
	// uart_putchar('G');
	// uart_putchar('I');
	// uart_putchar('N');

	//Scheduler test initialisations
	scheduler_init();
	
	//create_task(task1, "Task69", 1024);

	_enable_interrupts();
	timerInit(3*LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);
	// create_task(task2, "Task2", 1024);
	// create_task(task3, "Task2", 1024);
	// while (1)
	// {
	// 	printf("%c", x + '0');
	// }

        while(1) {
                printf("IDLEinMain!");
                delay(250);
        }
}
