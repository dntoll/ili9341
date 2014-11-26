#include "ili9341.h"


ili9341::ili9341() {
	int fd = wiringPiSPISetup(spiChannel, spiSpeed);
        if (fd <= -1) {
            
        }
	wiringPiSetup();
	pinMode(RST, OUTPUT);
        pinMode(DC, OUTPUT);
        if (LED == 1) {
        	pinMode(LED, PWM_OUTPUT);
        } else {
        	pinMode(LED, OUTPUT);
        }
}


void ili9341::clearScreen() {
	for(int i = 0;i < buffersize; i++)
		drawBuffer[i] =  0x00;
}
