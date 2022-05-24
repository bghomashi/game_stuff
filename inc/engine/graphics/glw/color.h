#ifndef __OGL_COLOR_H__
#define __OGL_COLOR_H__

namespace OGL {
	struct Color {
		float r, g, b, a;

		Color operator+(const Color& o) const;
		Color& operator+=(const Color& o);
		Color& operator=(const Color& o);
		Color operator*(const float o) const;
		Color& operator*=(const float o);
		bool operator==(const Color& o) const;

		static const Color White;
		static const Color WhiteSmoke;
		static const Color Gainsboro;
		static const Color LightGray;
		static const Color Silver;
		static const Color DarkGray;
		static const Color Gray;
		static const Color DimGray;
		static const Color DimDimGray;
		static const Color Black;
		static const Color Red;
		static const Color Orange;
		static const Color FireBrick;
		static const Color Green;
		static const Color Blue;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Yellow;
		static const Color Lime;
		static const Color DarkGreen;
		static const Color Purple;
		static const Color DarkViolet;
		static const Color Indigo;
		static const Color SkyBlue;
	};
}

#endif