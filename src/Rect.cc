#include "Rect.h"
#include <iostream>


using namespace std;

Rect::Rect(int dx, int dy, int dw, int dh) {
	x = dx;
	y = dy;
	width = dw;
	height = dh;

}

bool Rect::intersect(const Rect &other) const {
	if (other.left() >= right()) {
		return false;
	}
	if (left() >= other.right()) {
		return false;
	}
	if (other.bottom() >= top()) {
		return false;
	}
	if (bottom() >= top()) {
		return false;
	}
	return true;
}

void Rect::merge(const Rect &other) {
	int nx = x;
	int ny = y;
	if ( other.left() < left()) {
		nx = left();
	}
	if ( other.bottom() < bottom()) {
		ny = other.bottom();
	}
	if (other.right() > right()) {
		width = other.right() - nx;
	} else {
		width = right() - nx;
	}
	if (other.top() > top()) {
		height = other.top() - ny;
	} else {
		height = top() - ny;
	}
	x = nx;
	y = ny;
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
