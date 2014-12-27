#pragma once

#include <vector>
#include "Rect.h"
#include "Color.h"
#include "ili9341.h"


class BackBuffer {
public:
	BackBuffer(ili9341 &lcd);
	void clearScreen();
	void fillBox(const Rect &screen, const Color &fill);
	void flush();
private:
	static const int buffersize = ili9341::WIDTH*ili9341::HEIGHT*2; //two bytes per pixel
	std::vector<Rect> dirtyRects;
	unsigned char backBuffer[ili9341::WIDTH][ili9341::HEIGHT][2];
	unsigned char writeBuffer[buffersize];
	ili9341 &screen;
};
