#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <cstdint>
#include "glw/texture2d.h"

namespace OGL {
	class RenderBuffer {
		friend class FrameBuffer;

		std::uint32_t _rnd_id;
	public:
		void Create();
		void Destroy();
		void Bind();
		void Storage(std::uint32_t width, std::uint32_t height, std::uint32_t format);
	};
	class FrameBuffer {
		std::uint32_t _frm_id;
	public:
		static FrameBuffer GetDefault();

		void Create();
		void Destroy();

		bool IsComplete() const;
		void AttachColorBuffer(Texture2D& texture);
		void AttachColorBuffer(RenderBuffer& renderbuffer);
		void AttachDepthBuffer(Texture2D& texture);
		void AttachDepthBuffer(RenderBuffer& renderbuffer);
		void AttachStencilBuffer(Texture2D& texture);
		void AttachStencilBuffer(RenderBuffer& renderbuffer);
		void AttachDepthStencilBuffer(Texture2D& texture);
		void AttachDepthStencilBuffer(RenderBuffer& renderbuffer);

		void Bind() const;
		void Unbind() const;
	};
}

#endif