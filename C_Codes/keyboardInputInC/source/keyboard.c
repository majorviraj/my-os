#include <stdOutput.h>
#include <stdInput.h>

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

void kernel_main2()
{
    UsbInitialise();
        frameBufferSetup(1024, 768, 16);
    printf("\n\n\n\n\n\n\n\nyolyolo!\n\n");

    char character = 0;
    int x, y = 0;


    while (1)
    {

        do
        {
            keyboardInit();
            character = KeyboardGetChar();
        } while (character == 0);

        printf("%x", character);
    }
}

void kernel_main()
{
	char* test = "yoko\nooyyoyo\tyooyoy";

	UsbInitialise();
	frameBufferSetup(1920, 1080, 16);
	printf(test);
	char n = '\n';
	printf("%c", n);
	int i = 0;
	printf("Width, %i\n", frameBufferData);
	while(1) {
		printf("New line:");
		char x[20];
		memset(x, 0, 20);
		scanf("%20s", &x);
		printf("%s \n", x);
		i++;
	}
}