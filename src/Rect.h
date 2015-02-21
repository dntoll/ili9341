#pragma once

class Rect {
	public:

	int x, y, width, height;
	int left() const { return x; }
	int right() const { return x + width; }
	int bottom() const { return y; }
	int top() const { return y + height; }
	bool intersect(const Rect &other) const;
	void merge(const Rect &other);

	Rect(int dx, int dy, int dw, int dh);
	Rect crop(int minX, int minY, int maxX, int maxY) const;
};
