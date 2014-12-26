#pragma once


class Color {
	public:
		unsigned int r, g, b;
		Color(unsigned int dr, unsigned int dg, unsigned int db) {
			r = dr;
			g = dg;
			b = db;
		}

		static Color BLACK;
};
