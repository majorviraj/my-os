extern int frameBufferInit(int, int, int);
extern void UsbInitialise();
extern void keyboardInit();
extern char KeyboardGetChar();
extern int drawCharacter(int, int, int);

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
    char character = 0;
    int x, y = 0;

    frameBufferSetup(1024, 768, 16);
    
    while(1){
        
        do {
        

            character = KeyboardGetChar();
        }while( character == 0);

        x += drawCharacter(character, x, y);

        if (x > 1023) {
            x = 0;
            y++;
        }

        if (y > 767) {
            y = 0;
        }

    }
}