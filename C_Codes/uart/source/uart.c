#include <stdOutput.h>
#include <stdInput.h>
#include <lib_uart.h>
#include <timer.h>

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
	char *test = "yoko\nooyyoyo\tyooyoy";

	_enable_interrupts();

	// frameBufferSetup(1024, 768, 16);
	uart_init();

	// printf("Full array: ");
	uart_putchar('B');
	uart_putchar('E');
	uart_putchar('G');
	uart_putchar('N');
	while (1)
	{

		/*Following block polls the UART registers to check for input and prints them.
	* Do not use this unless UART interrupt controlled input is not working.
	*/
		//     while (1)
		//     {
		//         if (AUX_MU_LSR_REG & 0x01)
		//             break;
		//     }
		//     char x = (AUX_MU_IO_REG);
		//     printf("Hex value: %x     \t", x);
		//     printf("Char : %c\n", x);
		// }

		// if (!queue_is_empty(uart_queue))
		// {
		//     printf("%c", dequeue(uart_queue));
		// }

		if (!queue_is_empty(uart_queue))
		{
			uart_putchar('F');
			uart_putchar('Q');
			uart_putchar(':');
			uart_putchar(' ');
			delay(15000);
			// while (uart_queue->size >= 5)
			// {
			// 	// printf("%c  ", uart_queue->pointer[k]);
			// 	uart_putchar(dequeue(uart_queue));
			// 	// uart_putchar(uart_queue->pointer[k]);
			// 	// uart_putchar(' ');
			// }
			// delay(10000);
			while (!queue_is_empty(uart_queue))
			{
				// printf("%c  ", uart_queue->pointer[k]);
				uart_putchar(dequeue(uart_queue));
				// uart_putchar(uart_queue->pointer[k]);
				// uart_putchar(' ');
			}
			// printf("\nDone printing array outside while1\n");
			uart_putchar('S');
			uart_putchar('z');
			uart_putchar(':');
			uart_putchar((char)uart_queue->size);
			uart_putchar('\t');
			uart_putchar('D');
			uart_putchar('O');
			uart_putchar('N');
			uart_putchar('E');
			uart_putchar('\n');
		}
	}
}
