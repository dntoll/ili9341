#pragma once


class Color {
	public:
		unsigned int r, g, b;
		Color(unsigned int dr, unsigned int dg, unsigned int db);

		unsigned char get16bitHigh() const;
		unsigned char get16bitLow() const;

		static Color BLACK;
		static Color WHITE;
};


