#ifndef UART_H
#define UART_H

/*Code for enabling the MiniUART for debugging purposes. 
* Uses the pin8 (GPIO 14) as the Tx pin for UART.
* Uses the pin10 (GPIO 15) as the Rx pin for UART. 
* Call MiniUartInit function first to initialise the UART
* on GPIO 14 and 15 and use them for alternate function, AUX peripheral.
*/

#include <assemblyFunctions.h>
#include <stdarg.h>
#include <intTypes.h>
#include <rpiGpio.h>

#define AUX_BASE	( PERIPHERAL_BASE + 0x215000 )
#define HWREG(x)	(*((volatile uint32_t *)(x)))

#define AUX_MINI_UART_ENABLE 		(1<<0)
#define AUX_ENABLE_SPI0 		(1<<1)
#define AUX_ENABLE_SPI1 		(1<<2)

#define SYSTEM_FREQUENCY    250000000

#define GPFSEL1 	(HWREG(0x20200004))
#define GPSET0  	(HWREG(0x2020001C))
#define GPCLR0  	(HWREG(0x20200028))
#define GPPUD       	(HWREG(0x20200094))
#define GPPUDCLK0   	(HWREG(0x20200098))

//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1
//alt function 5 for uart1
//alt function 0 for uart0

//((250,000,000/115200)/8)-1 = 270

//Register definitions
#define AUX_ENABLES     (HWREG(0x20215004))
#define AUX_MU_IO_REG   (HWREG(0x20215040))
#define AUX_MU_IER_REG  (HWREG(0x20215044))
#define AUX_MU_IIR_REG  (HWREG(0x20215048))
#define AUX_MU_LCR_REG  (HWREG(0x2021504C))
#define AUX_MU_MCR_REG  (HWREG(0x20215050))
#define AUX_MU_LSR_REG  (HWREG(0x20215054))
#define AUX_MU_MSR_REG  (HWREG(0x20215058))
#define AUX_MU_SCRATCH  (HWREG(0x2021505C))
#define AUX_MU_CNTL_REG (HWREG(0x20215060))
#define AUX_MU_STAT_REG (HWREG(0x20215064))
#define AUX_MU_BAUD_REG (HWREG(0x20215068))

#define IRQ_BASIC 	(HWREG(0x2000B200))
#define IRQ_PEND1 	(HWREG(0x2000B204))
#define IRQ_PEND2 	(HWREG(0x2000B208))
#define IRQ_FIQ_CONTROL (HWREG(0x2000B210))
#define IRQ_ENABLE1 	(HWREG(0x2000B210))
#define IRQ_ENABLE2 	(HWREG(0x2000B214))
#define IRQ_ENABLE_BASIC (HWREG(0x2000B218))
#define IRQ_DISABLE1 	(HWREG(0x2000B21C))
#define IRQ_DISABLE2 	(HWREG(0x2000B220))
#define IRQ_DISABLE_BASIC (HWREG(0x2000B224))

volatile uint32_t uart_queue_size;
volatile uint32_t uart_queue_front;
volatile uint32_t uart_queue_rear;
volatile uint32_t uart_queue_size;
volatile char uart_queue_arr[20];


void uart_putchar(uint32_t c);
void uart_irq_handler (void);
void uart_init();
char uart_interrupt_deque();

#endif