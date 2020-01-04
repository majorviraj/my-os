#include <stdOutput.h>
#include <stdInput.h>
#include <scheduler.h>
#include <lib_uart.h>
#include <timer.h>

uint8_t x = 3;

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

void task1() {
	while(1) {
		// uart_putchar("1");
		x = 1;
	}
}

void task2() {
	while(1) {
		// uart_putchar("2");
		x = 2;
	}
}

void task3 () {
	while (1)
	{
		if (x !=0) {
			uart_putchar(x);
		}
		x = 0;
	}
}


void kernel_main()
{
	char *test = "yoko\nooyyoyo\tyooyoy";

	_enable_interrupts();

	// frameBufferSetup(1024, 768, 16);
	uart_init();

	// printf("Full array: ");
	uart_putchar('B');
	uart_putchar('E');
	uart_putchar('G');
	uart_putchar('I');
	uart_putchar('N');

	//Scheduler test initialisations
	timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);
	
	scheduler_init();
	create_task(task1, "Task1", 1024);
	create_task(task2, "Task2", 1024);
	create_task(task3, "Task2", 1024);
	while (1)
	{
		uart_putchar(x);
		
	}
}
