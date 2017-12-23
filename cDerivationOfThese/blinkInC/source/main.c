#define GPIO_BASE 0x20200000UL

volatile unsigned int* gpioFSEL = GPIO_BASE + 16;


int main() {

    while (1) {
        *gpioFSEL |= (1<<21);
    }
    return 0;

}

void exit (int code) {
    while(1);
}