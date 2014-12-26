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

void ili9341::test() {

	int x = 200;
	int y = 50;
	int width =  100;
	int height = 200;

	for (int i = 0; i < buffersize; i++) {
		writeBuffer[i] = rand()%255;
	}

	adressSet(y, x, height, width);
	digitalWrite(DC, 1);

	int maxWriteSize = 2048;
	int bytesToWrites = width * height * 2;
	int numIterations = bytesToWrites / maxWriteSize;
	int leftovers = bytesToWrites % maxWriteSize;
	unsigned char *p = (unsigned char *)&writeBuffer;

	for (int i = 0; i< numIterations; i++) {
		if (wiringPiSPIDataRW(spiChannel, p + i * maxWriteSize, maxWriteSize) == -1) {
			printf("SPI failed wiringPiSPIDataRW");
		}
	}


	if (wiringPiSPIDataRW(spiChannel, p + numIterations * maxWriteSize, leftovers) == -1) {
		printf("SPI failed wiringPiSPIDataRW");
	}

}

void ili9341::clearScreen() {
	fillBox(0, 0, WIDTH, HEIGHT, 0, 0, 0);
}

void ili9341::flush() {
	//writeToBuffer(0, 0, WIDTH, HEIGHT);

	for(int i=0; i < dirtyRects.size(); i++) {
		writeToBuffer(dirtyRects[i].x,
					dirtyRects[i].y,
					dirtyRects[i].w,
					dirtyRects[i].h);
	}
	dirtyRects.clear();


	//copy to fb
	/*for (int x=0; x < WIDTH; x++) {
		for (int y=0; y < HEIGHT; y++) {
			int i = toIndex(x, y);
			frontBuffer[i*2] = backBuffer[i*2];
			frontBuffer[i*2+1] = backBuffer[i*2+1];
		}
	}*/
}



void ili9341::writeToBuffer(int x, int y, int width, int height) {
	//copy bb to wb
	int index = 0;

	//build up write buffer
	//this is draw order dependent
	//x is reversed
	for (int dx=width-1; dx >= 0; dx--) {
		for (int dy=0; dy < height; dy++) {
			int to =  index*2;  //two bytes per pixel

			//here we turn it around since i mounted it wrong
			int bx = x + dx;
			int by = y + dy;
			if (bx >= 0 && bx < WIDTH &&
				by >= 0 && by < HEIGHT) {
					writeBuffer[to]   = backBuffer[bx][y + dy][0];
					writeBuffer[to+1] = backBuffer[bx][y + dy][1];
			}
			index++;
		}
	}

	//this one must be called in the wrong order, since we use the screen in landscape mode
	// x <-swap-> y
	//x is reversed WIDTH - x -1 -width
	adressSet(y, WIDTH - x -1 -width, height, width);

	//push buffer to screen
	digitalWrite(DC, 1);

	int maxWriteSize = 2048;
	int bytesToWrites = width * height * 2;
	int numIterations = bytesToWrites / maxWriteSize;

	for (int i = 0; i< numIterations; i++) {
		unsigned char *p = (unsigned char *)&writeBuffer;
		if (wiringPiSPIDataRW(spiChannel, p + i * maxWriteSize, maxWriteSize) == -1) {
			printf("SPI failed wiringPiSPIDataRW");
		}

	}

	int leftovers = bytesToWrites % maxWriteSize;
	unsigned char *p = (unsigned char *)&writeBuffer;
	if (wiringPiSPIDataRW(spiChannel, p + numIterations * maxWriteSize, leftovers) == -1) {
		printf("SPI failed wiringPiSPIDataRW");
	}


}

void ili9341::LCD_Write_DATA(unsigned char data) {
	digitalWrite(DC, 1);
	if (wiringPiSPIDataRW(spiChannel, &data, 1) == -1) {
		printf("SPI failed lcd_data");
	}
	
}

void ili9341::LCD_Write_COM(unsigned char com) {
	digitalWrite(DC, 0);
	if (wiringPiSPIDataRW(spiChannel, &com, 1) == -1) {
		printf("SPI failed lcd_cmd");
	}
}

void ili9341::fillBox(int x, int y, int width, int height, int r, int g, int b)
{
	// rrrrrggggggbbbbb
	int bch=((r&248)|g>>5);
	int bcl=((g&28)<<3|b>>3);
	int color = (bch<<8) | bcl;


	int left = WIDTH;
	int right = 0;
	int top = HEIGHT;
	int bottom = 0;

	for (int dx=0; dx < width && x+dx < WIDTH; dx++) {
		for (int dy=0; dy < height && y+dy < HEIGHT; dy++) {
			backBuffer[x+dx][y+dy][0] = (unsigned char) bch;
			backBuffer[x+dx][y+dy][1] = (unsigned char) bcl;
			if (x+dx < left)
				left = x + dx;
			if (y+dy < top)
				top = y + dy;
			if (x+dx > right)
				right = x + dx;
			if (y+dy > bottom)
				bottom = y + dy;
		}
	}
	
	dirtyRects.push_back(Rect(left, top, right - left, bottom - top));

}

void ili9341::adressSet( int x, int y, int width, int height)
{
	//https://github.com/luckasfb/lcm_drivers/blob/master/alcatel_ot_903d_jrd73_gb/lcm/ili9341/ili9341.c
	unsigned int x0 = x;
	unsigned int y0 = y;
	unsigned int x1 = x0 + width - 1;
	unsigned int y1 = y0 + height - 1;

	LCD_Write_COM(0x2a);
	LCD_Write_DATA(x0 >> 8);
	LCD_Write_DATA(x0);
	LCD_Write_DATA(x1 >> 8);
	LCD_Write_DATA(x1);

	LCD_Write_COM(0x2b);
	LCD_Write_DATA(y0 >> 8);
	LCD_Write_DATA(y0);
	LCD_Write_DATA(y1 >> 8);
	LCD_Write_DATA(y1);

	LCD_Write_COM(0x2C);         				 
}


