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

	dx = x > maxX ? maxX : x;
	dy = y > maxY ? maxY : y;

	int dw = width + x < minX ? 0 : width;
	int dh = height + x < minY ? 0 : height;

	dw = dw + x > maxX ? maxX-width : dw;
	dh = dh + x > maxY ? maxY-height : dh;

	return Rect(dx, dy, dw, dh);
}
