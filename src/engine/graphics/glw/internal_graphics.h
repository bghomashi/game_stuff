#ifndef __INTERNAL_GRAPHICS_H__
#define __INTERNAL_GRAPHICS_H__

#include <cstdint>
#include <array>
#include "graphics.h"

namespace OGL {

    struct ConvertToGL {
        static const std::array<std::uint32_t, Filter::NUM> g_texture_filters;
        static const std::array<std::uint32_t, InternalFormat::NUM> g_internal_formats;
        static const std::array<std::uint32_t, PixelFormat::NUM> g_pixel_formats;
        static const std::array<std::uint32_t, PixelType::NUM> g_pixel_types;
        static const std::array<std::uint32_t, Wrapping::NUM> g_texture_wrapping;
        static const std::array<std::uint32_t, DepthTest::NUM> g_depth_test;
        static const std::array<std::uint32_t, ClearBit::NUM> g_clear_bits;
		static const std::array<std::uint32_t, BlendFunc::NUM> g_blend_func;
		static const std::array<std::uint32_t, BlendEquation::NUM> g_blend_eq;
    };

}


#endif
