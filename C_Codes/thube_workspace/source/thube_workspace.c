#include <assemblyFunctions.h>
#include <interrupt.h>
#include <intTypes.h>
#include <timer.h>
#include <stdOutput.h>
#include <tgmath.h>

#define SCALE 35
#define XOFFSET 200
#define YOFFSET 500
#define PI 3.14159265

void frameBufferSetup(int width, int height, int bitDepth)
{
	uint32_t error = frameBufferInit(width, height, bitDepth);

	if (error == 0)
	{
		return;
	}

	else
	{
		// error handler
		// gpioBlink(200, 10);
	}
}


// double getX(double theta1, double theta2)
// {
// 	printf("Inside getX");
// 	double retVal = SCALE * (9 * cos(theta1) + 4 * cos(theta1 + theta2));
// 	printf("Inside getX");
// 	printf("x: %d\t", (int)retVal);
// 	return retVal;
// }

// double getY(double theta1, double theta2)
// {
// 	printf("Inside getY");
// 	double retVal = SCALE * (9 * sin(theta1) + 4 * sin(theta1 + theta2));
// 	printf("y : %d \n", (int)retVal);
// 	return retVal;
// }


void kernel_main()
{

	// _enable_interrupts();
	// uart_init();
	// uart_putchar('Q');
	// uart_putchar('X');

	frameBufferSetup(1920, 1080, 16);
	setStartPosition(0, 0);
	setCursor(0);

	// timerInit(LOAD_VALUE_1S_1_PRESCALLAR, Bit23, TIMER_CONTROL_PRESCALLAR_1);
	printf("yoyoyoyoyoyoyoyoyoyoyoyoyoyoyo\n");
	double retVal = SCALE * (9 * cos(0) + 4 * cos(0));
	printf("init test: %i\t", (int)retVal);

	// double newVal = SCALE * (9 * cos(theta1) + 4 * cos(theta1 + theta2));
	// printf("new test: %i\t\n", (int)newVal);

	// x = 9*cos(theta1) + 4*cos(theta1 + theta2);
	// y = 9*sin(theta1) + 4*sin(theta1 + theta2);

	// theta1 between -90 to 90
	// theta2 between -120 to 120

	// setForeColour(0xFFFF);
	// for (uint32_t x = 50; x < 250; x++)
	// {
	// 	for (uint32_t y = 50; y < 250; y++)
	// 	{
	// 		drawPixel(x, y);
	// 	}
	// }

	setForeColour(0xFFFF);
	printf("entering for");
	double valX = 0;
	double valY = 0;
	for (double x = -1.57; x < 1.57; x += 0.01)
	{
		// printf("IN FOR");
		for (double y = -2; y < 2; y += 0.01)
		{
			// drawPixel(OFFSET + (int)getX(x, y), OFFSET + (int)getY(x, y));
			// printf("thetaX: %i\t thetaY: %i\n ", (int)x, (int)y);
			valX = (double)XOFFSET + (double)((double)SCALE * ((double)9 * cos((double)x)));
			valY = YOFFSET + (double)(SCALE * (9 * sin(x) + 4 * sin(x + y)));
			// printf("ValX: %i ValY: %i\n ", (int)valX, (int)valY);
			drawPixel((uint16_t)valX, (uint16_t)valY);
		}
	}
	setForeColour(0xFFFF);
	printf("END OF FILE");
	
	while (1)
	{
	}
}
