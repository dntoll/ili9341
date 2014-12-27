#include "Color.h"

Color Color::BLACK = Color(0,0,0);


Color::Color(unsigned int dr, unsigned int dg, unsigned int db) {
	r = dr;
	g = dg;
	b = db;
}

// rrrrrggggggbbbbb
unsigned char Color::get16bitHigh() const {
	return ((r & 248) | g >> 5);
}
// rrrrrggggggbbbbb
unsigned char Color::get16bitLow() const {
	return ((g & 28) << 3 | b >> 3);
}
