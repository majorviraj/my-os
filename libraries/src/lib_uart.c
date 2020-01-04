#include <lib_uart.h>
#include <stdOutput.h>
#include <interrupt.h>
#include <lib_queue.h>

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270



void uart_interrupt_enqueue (char);

void uart_init() {

	uart_queue = (queue_t*)create_queue(20);

	AUX_ENABLES = 1;
	AUX_MU_IER_REG = 0;
	AUX_MU_CNTL_REG = 0;
	AUX_MU_LCR_REG = 3;
	AUX_MU_MCR_REG = 0;
	AUX_MU_IER_REG = 0x5; //Enable rx interrupts
	AUX_MU_IIR_REG = 0xC6;
	AUX_MU_BAUD_REG = 270;

	IRQController->enableIRQ1 |= (1<<29); //aux interrupt enabled


	//GPIO function selection
	unsigned int ra;
	ra = GPFSEL1;
    	ra &= ~(7<<12); //gpio14
    	ra |=  (2<<12);    //alt5
    	ra &= ~(7<<15); //gpio15
    	ra |=  (2<<15);    //alt5
	GPFSEL1 = ra;

	GPPUD = 0;
	delayCycles(300);
	GPPUDCLK0 = (1<<14)|(1<<15);
	delayCycles(300);
	GPPUDCLK0 = 0;

	AUX_MU_CNTL_REG = 3; //Enable UART fifo's.
}

void uart_putchar(uint32_t c)
{
	while(1)
	{
		if(AUX_MU_LSR_REG & 0x20) break;
	}
	AUX_MU_IO_REG = c;
}

void uart_irq_handler (void)
{
	char received_char;

	//an interrupt has occurred, find out why
	while(1) //resolve all interrupts to uart
	{
		if((AUX_MU_IIR_REG & 1)==1) break; //no more interrupts
		if((AUX_MU_IIR_REG & 6)==4)
		{
        		//receiver holds a valid byte
        		received_char = (char)AUX_MU_IO_REG; //read byte from rx fifo
			// printf("Printed from IRQ Handler: %c\n", received_char);
			enqueue(uart_queue, received_char);
		}
	}
}


