#include "ili9341.h"
#include <unistd.h>
#include <stdio.h>      /* printf, fopen */
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <wiringPiSPI.h>
#include <wiringPi.h>

#include <iostream>


using namespace std;

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


void ili9341::writeToBuffer(const Rect &pos, unsigned char *writeBuffer) {


	//this one must be called in the wrong order, since we use the screen in landscape mode
	// x <-swap-> y
	//x is reversed WIDTH - x -1 -width
	adressSet(pos.y, WIDTH - pos.x -pos.width, pos.height, pos.width);

	//push buffer to screen
	digitalWrite(DC, 1);

	int maxWriteSize = 1024;
	int bytesToWrites = pos.width * pos.height * 2;
	int numIterations = bytesToWrites / maxWriteSize;
	int leftovers = bytesToWrites % maxWriteSize;

	cout << "bytesToWrites "  << bytesToWrites << endl;
	cout << "numIterations " << numIterations << endl;
	cout << "leftovers " << leftovers << endl;

	for (int i = 0; i< numIterations; i++) {
		unsigned char *p = writeBuffer;
		if (wiringPiSPIDataRW(spiChannel, p + i * maxWriteSize, maxWriteSize) == -1) {
			printf("SPI failed wiringPiSPIDataRW");
		}

	}



	unsigned char *p = (unsigned char *)&writeBuffer;
	if (wiringPiSPIDataRW(spiChannel, p + numIterations * maxWriteSize, leftovers) == -1) {
		printf("SPI failed wiringPiSPIDataRW");
	} else {
		//cout << "wrote leftovers " << leftovers << endl;
	}



	cout << "wrote "  << numIterations * maxWriteSize +  leftovers << endl;
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


