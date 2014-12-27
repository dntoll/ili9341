#include "Rect.h"


Rect::Rect(int dx, int dy, int dw, int dh) {
	x = dx;
	y = dy;
	width = dw;
	height = dh;

}


Rect Rect::crop(int minX, int minY, int maxX, int maxY) const {
	int dx = x < minX ? minX : x;
	int dy = y < minY ? minY : y;

	dx = dx > maxX ? maxX : dx;
	dy = dy > maxY ? maxY : dy;

	int dw = width + x < minX ? 0 : width;
	int dh = height + x < minY ? 0 : height;

	dw = dw + x > maxX ? maxX-dx-1 : dw;
	dh = dh + y > maxY ? maxY-dy-1 : dh;

	return Rect(dx, dy, dw, dh);
}
