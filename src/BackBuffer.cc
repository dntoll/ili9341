#include "BackBuffer.h"
#include <iostream>


using namespace std;
BackBuffer::BackBuffer(ili9341 &lcd) : screen(lcd) {

}

void BackBuffer::clearScreen() {
	fillBox(Rect(0, 0, ili9341::WIDTH, ili9341::HEIGHT), Color::BLACK);
}

void BackBuffer::flush() {
	dirtyRects.clear();
	dirtyRects.push_back(Rect(0,0,ili9341::WIDTH, ili9341::HEIGHT));

	for(int i=0; i < dirtyRects.size(); i++) {

		//copy bb to wb
		int index = 0;

		//build up write buffer
		//this is draw order dependent
		//x is reversed
		for (int dx=dirtyRects[i].width-1; dx >= 0; dx--) {
			for (int dy=0; dy < dirtyRects[i].height; dy++) {

				int to =  index*2;  //two bytes per pixel

				//here we turn it around since i mounted it wrong
				int bx = dirtyRects[i].x + dx;
				int by = dirtyRects[i].y + dy;
				if (bx >= 0 && bx < ili9341::WIDTH &&
					by >= 0 && by < ili9341::HEIGHT) {
						writeBuffer[to]   = backBuffer[bx][by][0];
						writeBuffer[to+1] = backBuffer[bx][by][1];
				}
				index++;
			}
		}

		//cout << "index " << index << endl;
		screen.writeToBuffer(dirtyRects[i], writeBuffer);

	}
	dirtyRects.clear();
}

void BackBuffer::fillBox(const Rect &screen, const Color &fill)
{
	unsigned char bch= fill.get16bitHigh();
	unsigned char bcl= fill.get16bitLow();

	Rect onScreen = screen.crop(0,0, ili9341::WIDTH, ili9341::HEIGHT);

	for (int dx=onScreen.x; dx < onScreen.x + onScreen.width; dx++) {
		for (int dy=onScreen.y; dy < onScreen.y + onScreen.height; dy++) {
			backBuffer[dx][dy][0] = bch;
			backBuffer[dx][dy][1] = bcl;
		}
	}

	dirtyRects.push_back(onScreen);

}
