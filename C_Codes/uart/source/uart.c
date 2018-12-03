#include <stdOutput.h>
#include <stdInput.h>
#include <uart.h>
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
    uart_init();
    _enable_interrupts();

    //To toggle GPIO
    setLEDasOutput();
    // timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);

    frameBufferSetup(1024, 768, 16);
    printf(test);
    char n = '\n';
    printf("%c", n);
    int i = 0;
    printf("Width, %i\n", frameBufferData);
    uart_putchar('U');
    uart_putchar('A');
    uart_putchar('R');
    uart_putchar('T');

    printf("\n");
    uart_interrupt_enqueue('V');
    uart_interrupt_enqueue('X');
    uart_interrupt_enqueue('L');

    printf("Full array: ");
    for (int k = 0; k < 20; k++)
    {
        printf("%c  ", uart_queue_arr[k]);
    }
    printf("\nDone printing array outside while1\n");

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

        if (uart_queue_size != 0)
        {
            printf("%c", uart_interrupt_dequeue());
        }

    }
}