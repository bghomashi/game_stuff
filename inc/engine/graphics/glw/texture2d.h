#ifndef __OGL_TEXTURE2D_H__
#define __OGL_TEXTURE2D_H__

#include <cstdint>
#include "glw/color.h"

namespace OGL {
	class Texture2D {
		friend class FrameBuffer;

		std::uint32_t _tex_id;
	public:
		void Create();
		void Destroy();

		void Data(std::uint32_t internal_format, std::uint32_t width, std::uint32_t height, std::uint32_t pixel_format, std::uint32_t pixel_type, const void* pixels) const;
		void SetMinFilter(std::uint32_t filter);
		void SetMagFilter(std::uint32_t filter);
		void SetWrapping(std::uint32_t wrap_mode);
		void SetBorderColor(const Color& color);

		void Bind(unsigned id) const;
		void Unbind(unsigned id) const;

	};
}

#endif