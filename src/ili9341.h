#pragma once
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include <vector>


class ili9341 {

public:
	ili9341();

	void clearScreen();

	//write backbuffer to screen
	void flush();

	void setBrightness(unsigned char lightLevel);
	void fillBox(int x, int y, int width, int height, int r, int g, int b);

	static const int WIDTH = 320;
	static const int HEIGHT = 240;
private:
	void adressSet( int x, int y, int w, int h);
	void LCD_Write_DATA(unsigned char data);
	void LCD_Write_COM(unsigned char com);
	void writeToBuffer(int x, int y, int width, int height);

	static const unsigned char DC   = 4;// # gpio pin 16 = wiringpi no. 4 (BCM 23)
	static const unsigned char RST  = 5;// # gpio pin 18 = wiringpi no. 5 (BCM 24)
	static const unsigned char LED  = 1;// # gpio pin 12 = wiringpi no. 1 (BCM 18)

	//# SPI connection, these are only for documentation...
	static const unsigned char SCE  = 10;// # gpio pin 24 = wiringpi no. 10 (CE0 BCM 8) 
	static const unsigned char SCLK = 14;// # gpio pin 23 = wiringpi no. 14 (SCLK BCM 11)
	static const unsigned char DIN  = 12;// # gpio pin 19 = wiringpi no. 12 (MOSI BCM 10)


	static const int spiChannel = 0;
	static const int spiSpeed = 32000000;
	static const int buffersize = WIDTH*HEIGHT*2; //two bytes per pixel

	struct Rect {
		int x, y, w, h;
		Rect(int dx, int dy, int dw, int dh) {
			x = dx;
			y = dy;
			w = dw;
			h = dh;
		}
	};



	std::vector<Rect> dirtyRects;

	//unsigned char frontBuffer[buffersize];
	unsigned char backBuffer[WIDTH][HEIGHT][2];
	unsigned char writeBuffer[buffersize];
	int fileDescriptor;

};
