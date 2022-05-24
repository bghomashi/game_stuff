#include "internal_graphics.h"
#include <glad/glad.h>

namespace OGL {

const std::array<std::uint32_t, Filter::NUM> ConvertToGL::g_texture_filters = {
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST, 
    GL_NEAREST_MIPMAP_LINEAR, 
    GL_LINEAR_MIPMAP_NEAREST, 
    GL_LINEAR_MIPMAP_LINEAR
};
const std::array<std::uint32_t, InternalFormat::NUM> ConvertToGL::g_internal_formats = {
    GL_RGB,
    GL_RGBA,

    GL_DEPTH_COMPONENT,
    GL_STENCIL_INDEX,
    GL_DEPTH24_STENCIL8,
};
const std::array<std::uint32_t, PixelFormat::NUM> ConvertToGL::g_pixel_formats = {
    GL_RGB,
    GL_RGBA,

    GL_DEPTH_COMPONENT,
    GL_STENCIL_INDEX,
    GL_DEPTH_STENCIL,
};
const std::array<std::uint32_t, PixelType::NUM> ConvertToGL::g_pixel_types = {
    GL_UNSIGNED_BYTE,
    GL_FLOAT,

    GL_UNSIGNED_INT,
    GL_UNSIGNED_INT_24_8,

};
const std::array<std::uint32_t, Wrapping::NUM> ConvertToGL::g_texture_wrapping = {
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
};
const std::array<std::uint32_t, DepthTest::NUM> ConvertToGL::g_depth_test = {
	GL_ALWAYS,
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
};
const std::array<std::uint32_t, ClearBit::NUM> ConvertToGL::g_clear_bits = {
	GL_COLOR_BUFFER_BIT,
	GL_DEPTH_BUFFER_BIT, 
	GL_STENCIL_BUFFER_BIT,
	GL_ACCUM_BUFFER_BIT
};
const std::array<std::uint32_t, BlendFunc::NUM> ConvertToGL::g_blend_func = {
    GL_ZERO,				
    GL_ONE,					
    GL_SRC_COLOR, 			
    GL_ONE_MINUS_SRC_COLOR, 
    GL_DST_COLOR, 			
    GL_ONE_MINUS_DST_COLOR, 
    GL_SRC_ALPHA, 			
    GL_ONE_MINUS_SRC_ALPHA,	
    GL_DST_ALPHA, 			
    GL_ONE_MINUS_DST_ALPHA,
    GL_CONSTANT_COLOR, 		
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_CONSTANT_ALPHA, 			
    GL_ONE_MINUS_CONSTANT_ALPHA
};
const std::array<std::uint32_t, BlendEquation::NUM> ConvertToGL::g_blend_eq = {
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,

	GL_MIN,
	GL_MAX
};
}
