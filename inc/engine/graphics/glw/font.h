#ifndef __OGL_FONT_H__
#define __OGL_FONT_H__

#include <cstdint>
#include <vector>
#include <string>
#include "glw/color.h"

namespace OGL {
    class Font {
		struct Glyph {
			std::uint32_t tex_id;
			int size[2];
			int bearing[2];
			int advance;
		};

		std::vector<Glyph> _glyphs;
		std::uint32_t _height, _bearingY;
	public:
		bool Load(const std::string& fontname, std::uint32_t height);
		void RenderText(const std::string& text, float x, float y, const Color& color, float scale = 1) const;
		void Destroy();

		std::int32_t GetAdvance(int c) const;
		std::int32_t GetWidth(int c) const;
		std::int32_t GetHeight(int c) const;
		std::int32_t GetBearingX(int c) const;
		std::int32_t GetBearingY(int c) const;
		std::int32_t GetHeight() const;
		std::int32_t GetBearingY() const;
	};
}

#endif