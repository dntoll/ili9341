#include "ili9341.h"
#include <unistd.h>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */

ili9341::ili9341() {
	fileDescriptor = wiringPiSPISetup(spiChannel, spiSpeed);
	if (fileDescriptor <= -1) {
		 printf ("Error wiringPiSPISetup");
		 exit(EXIT_FAILURE);
	}
	wiringPiSetup();
	pinMode(RST, OUTPUT);
	pinMode(DC, OUTPUT);
	if (LED == 1) {
		pinMode(LED, PWM_OUTPUT);
	} else {
		pinMode(LED, OUTPUT);
	}

	digitalWrite(RST, 0);
	usleep(11000);
	digitalWrite(RST, 1);
        
	LCD_Write_COM(0xCB);  
	LCD_Write_DATA(0x39);
	LCD_Write_DATA(0x2C);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x34);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xCF);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0XC1);
	LCD_Write_DATA(0X30);

	LCD_Write_COM(0xE8);
	LCD_Write_DATA(0x85);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x78);

	LCD_Write_COM(0xEA);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);

	LCD_Write_COM(0xED);
	LCD_Write_DATA(0x64);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0X12);
	LCD_Write_DATA(0X81);

	LCD_Write_COM(0xF7);
	LCD_Write_DATA(0x20);

	LCD_Write_COM(0xC0);    //Power control
	LCD_Write_DATA(0x23);   //VRH[5:0]

	LCD_Write_COM(0xC1);    //Power control
	LCD_Write_DATA(0x10);   //SAP[2:0];BT[3:0]

	LCD_Write_COM(0xC5);    //VCM control
	LCD_Write_DATA(0x3e);   //Contrast
	LCD_Write_DATA(0x28);

	LCD_Write_COM(0xC7);    //VCM control2
	LCD_Write_DATA(0x86);   //--

	LCD_Write_COM(0x36);    // Memory Access Control
	LCD_Write_DATA(0x48);   //C8	   //48 68竖屏//28 E8 横屏

	LCD_Write_COM(0x3A);
	LCD_Write_DATA(0x55);

	LCD_Write_COM(0xB1);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x18);

	LCD_Write_COM(0xB6);    // Display Function Control
	LCD_Write_DATA(0x08);
	LCD_Write_DATA(0x82);
	LCD_Write_DATA(0x27);

	LCD_Write_COM(0x11);    //Exit Sleep
	usleep(120000);

	LCD_Write_COM(0x29);    //Display on
	LCD_Write_COM(0x2c);

	setBrightness(0);
}

void ili9341::setBrightness(unsigned char led_value) {
	if ( LED == 1) {
		if (0 <= led_value && led_value < 1023) {
			pwmWrite(LED, led_value);
		}
	} else {
		if (led_value == 0)
			digitalWrite(LED, 0);
		else
			digitalWrite(LED, 1);
	}
	sleep(1);
	
}

void ili9341::clearScreen() {
	fillBox(0, 0, 240, 320, 0, 0, 0);
}

void ili9341::flush() {
	//writeBuffer(0, 0, 240, 320);

	for (int i = 0; i < buffersize; i += 2) {
		if (frontbuffer[i*2] == backBuffer[i*2] && frontbuffer[i*2+1] == backBuffer[i*2+1]) {
			//ok
		} else {
			//find how long
			for (int j = i+2; j < buffersize; j += 2) {
				if (frontbuffer[j*2] == backBuffer[j*2] && frontbuffer[j*2+1] == backBuffer[j*2+1]) {
					//break and write
					int numBytes = j-i;



					break;
				}

			}

		}
	}

	//copy to fb
	for (int x=0; x < width; x++) {
		for (int y=0; y < height; y++) {
			int i = y * 240 + x;
			frontbuffer[i*2] = backBuffer[i*2];
			frontbuffer[i*2+1] = backBuffer[i*2+1];
		}
	}
}

void ili9341::writeBuffer(int x, int y, int width, int height) {
	Address_set(x, y, x+width, y+height);

	//push buffer
	digitalWrite(DC, 1);

	int maxWriteSize = 2048;
	int bytesToWrites = width * height * 2;
	int numIterations = bytesToWrites / maxWriteSize;

	for (int i = 0; i< numIterations; i++) {
		unsigned char *p = (unsigned char *)&backBuffer;
		if (wiringPiSPIDataRW(spiChannel, p + i * maxWriteSize, maxWriteSize) == -1) {
			printf("spi failed wiringPiSPIDataRW");
		}
	}
	int leftovers = bytesToWrites % maxWriteSize;
	unsigned char *p = (unsigned char *)&backBuffer;
	if (wiringPiSPIDataRW(spiChannel, p + numIterations * maxWriteSize, leftovers) == -1) {
		printf("spi failed wiringPiSPIDataRW");
	}


}

void ili9341::LCD_Write_DATA(unsigned char data) {
	digitalWrite(DC, 1);
	if (wiringPiSPIDataRW(spiChannel, &data, 1) == -1) {
		printf("spi failed lcd_data");
	}
	
}

void ili9341::LCD_Write_COM(unsigned char com) {
	digitalWrite(DC, 0);
	if (wiringPiSPIDataRW(spiChannel, &com, 1) == -1) {
		printf("spi failed lcd_cmd");
	}
}

void ili9341::setColor(int x, int y, int r, int g, int b)
{
	// rrrrrggggggbbbbb
	int bch=((r&248)|g>>5);
	int bcl=((g&28)<<3|b>>3);
	int color = (bch<<8) | bcl;
	Address_set(x, y, x, y);	
	LCD_Write_DATA(bch);
	LCD_Write_DATA(bcl);
	int i = y * 240 + x;
	frontbuffer[i*2] = bch;
	frontbuffer[i*2+1] = bcl;

}

void ili9341::fillBox(int x, int y, int width, int height, int r, int g, int b)
{
	// rrrrrggggggbbbbb
	int bch=((r&248)|g>>5);
	int bcl=((g&28)<<3|b>>3);
	int color = (bch<<8) | bcl;

	for (int x=0; x < width; x++) {
		for (int y=0; y < height; y++) {
			int i = y * 240 + x;
			backBuffer[i*2] = (unsigned char) bch;
			backBuffer[i*2+1] = (unsigned char) bcl;
		}
	}
	dirtyRects.push_back(Rect(x, y, width, height));
	
}

void ili9341::Address_set( int x1, int y1, int x2, int y2)
{
	LCD_Write_COM(0x2a);
	LCD_Write_DATA(x1 >> 8);
	LCD_Write_DATA(x1);
	LCD_Write_DATA(x2 >> 8);
	LCD_Write_DATA(x2);

	LCD_Write_COM(0x2b);
	LCD_Write_DATA(y1 >> 8);
	LCD_Write_DATA(y1);
	LCD_Write_DATA(y2 >> 8);
	LCD_Write_DATA(y2);

	LCD_Write_COM(0x2C);         				 
}


