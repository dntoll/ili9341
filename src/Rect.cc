#include "Rect.h"
#include <iostream>


using namespace std;

Rect::Rect(int dx, int dy, int dw, int dh) {
	x = dx;
	y = dy;
	width = dw;
	height = dh;

}


Rect Rect::crop(int minX, int minY, int maxX, int maxY) const {
	int dx = x;
	int dy = y;
	int dw = width;
	int dh = height;

	if (dx < minX) {
		dx = minX;
		dw = dw - (minX - x);
		if (dx + dw < minX) {
			dw = 0;
		}
	} else if (dx > maxX) {
		dx = maxX;
		dw = 0;
	}
	if (dx + dw > maxX) {
		dw = maxX - dx;
	}

	if (dy < minY) {
		dy = minY;
		dh = dh - (minY - y);

		if (dy + dh < minY) {
			dh = 0;
		}
	} else if (dy > maxY) {
		dy = maxY;
		dh = 0;
	}
	if (dy + dh > maxY) {
		dh = maxY - dy;
	}

	//cout << "Rect::crop    " << x << " " << y << " " << width << " " << height << " " << endl;
	//cout << "Rect::cropped " << dx << " " << dy << " " << dw << " " << dh << " " << endl;

	return Rect(dx, dy, dw, dh);
}
