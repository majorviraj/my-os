#include <stdOutput.h>
#include <stdInput.h>
#include <context.h>

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
	process_control_block[0].state = BLOCKED;
	process_control_block[1].state = BLOCKED;
	process_control_block[2].state = BLOCKED;
	// char* test = "yoko\nooyyoyo\tyooyoy";
	// UsbInitialise();
	// frameBufferSetup(1920, 1080, 16);
	// printf(test);
	// char n = '\n';
	// printf("%c", n);
	// int i = 0;
	// printf("Width, %i\n", frameBufferData);
	// while(1) {
	// 	printf("New line:");
	// 	char x[20];
	// 	memset(x, 0, 20);
	// 	scanf("%20s", &x);
	// 	printf("%s \n", x);
	// 	i++;
	// }
}
