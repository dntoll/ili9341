#pragma once

class Rect {
	public:

	int x, y, width, height;


	Rect(int dx, int dy, int dw, int dh);
	Rect crop(int minX, int minY, int maxX, int maxY) const;
};
