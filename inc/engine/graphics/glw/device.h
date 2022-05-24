#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "glw/color.h"
#include <cstdint>

namespace OGL {
	class Device {
	public:
		// static std::mutex s_context_mutex;

		static void Clear(std::uint32_t bits);

		static void SetClearColor(const Color& color);

		static void SetClearDepth(float d);
		static void SetDepthFunc(std::uint32_t func);			// depth function
		static void SetDepthMask(bool flag = true);			// write to depth buffer
		static void SetDepthTest(bool flag = true);			// test against depth buffer

		static void SetStencilMask(bool flag = true);			//
		static void SetStenciltest(bool flag = true);			// test against stencil

		static void SetBlending(bool flag = true);				// blend mode 
		static void SetBlendFunc(std::uint32_t src, std::uint32_t dst);			// set the blend func parameters
		static void SetBlendEqua(unsigned equa);

		static void SetScissor(bool flag = true);
		static void Scissor(int x, int y, std::uint32_t width, std::uint32_t height);								// scissor text

		static void SetSmoothLine(bool flag = true);
		static void SetLineWidth(unsigned width);
		static void SetPointSize(unsigned size);
		static void SetLineStipple(int factor, std::uint16_t pattern);
		static void SetViewport(int x, int y, std::uint32_t width, std::uint32_t height);
		static void GetViewport(int& x, int& y, std::uint32_t& width, std::uint32_t& height);
		static void SetMultisampling(bool flag = true);
	};
}

#endif