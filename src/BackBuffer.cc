#include "BackBuffer.h"

BackBuffer::BackBuffer(ili9341 &lcd) : screen(lcd) {

}

void BackBuffer::clearScreen() {
	fillBox(Rect(0, 0, ili9341::WIDTH, ili9341::HEIGHT), Color::BLACK);
}

void BackBuffer::flush() {

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

		screen.writeToBuffer(dirtyRects[i], writeBuffer);

	}
	dirtyRects.clear();
}

void BackBuffer::fillBox(const Rect &screen, const Color &fill)
{

	unsigned char bch= fill.get16bitHigh();
	unsigned char bcl= fill.get16bitLow();


	int left = ili9341::WIDTH;
	int right = 0;
	int top = ili9341::HEIGHT;
	int bottom = 0;


	for (int dx=0; dx < screen.width && screen.x + dx < ili9341::WIDTH; dx++) {
		for (int dy=0; dy < screen.height && screen.y + dy < ili9341::HEIGHT; dy++) {
			backBuffer[screen.x + dx][screen.y + dy][0] = bch;
			backBuffer[screen.x + dx][screen.y + dy][1] = bcl;

			//screen.crop(0, 0, WIDTH, HEIGHT)?
			if (screen.x + dx < left)
				left = screen.x + dx;
			if (screen.y + dy < top)
				top = screen.y + dy;
			if (screen.x + dx > right)
				right = screen.x + dx;
			if (screen.y + dy > bottom)
				bottom = screen.y + dy;
		}
	}

	dirtyRects.push_back(Rect(left, top, right - left, bottom - top));

}
