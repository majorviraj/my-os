#include<stdOutput.h>

int hh;
void frameBufferSetup(int width, int height, int bitDepth){
    int error = frameBufferInit(width, height, bitDepth);

    if (error == 0){
        return 0;
    }

    else {
        // error handler
    }
}

void kernel_main() {
    UsbInitialise();
    

    char character = 0;
    int x, y = 0;

    frameBufferSetup(1024, 768, 16);
    
    printf("\n\n\n\n\n\n\n\nyolyolo!\n\n");
    while(1){
           
        do {
        keyboardInit();
        character = KeyboardGetChar();
        }while( character == 0);

        printf("%c", character);
    }
}