
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glw/texture2d.h"
#include "internal_graphics.h"

#include <iostream>

#include <iostream>
namespace OGL {
void Texture2D::Create() {
	glGenTextures(1, &_tex_id);
}
void Texture2D::Destroy() {
	glDeleteTextures(1, &_tex_id);
	_tex_id = 0;
}


void Texture2D::Bind(unsigned unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _tex_id);
}
void Texture2D::Unbind(unsigned unit) const {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::Data(
	std::uint32_t internal_format,
	std::uint32_t width, std::uint32_t height,
	std::uint32_t pixel_format, std::uint32_t pixel_type,
	const void* pixels) const {
	glTexImage2D(
		GL_TEXTURE_2D,
		0,                                  // mipmap level
		ConvertToGL::g_internal_formats[internal_format],// internal format
		width, height,                      // dimensions
		0,                                  // always 0
		ConvertToGL::g_pixel_formats[pixel_format],      // (input) format
		ConvertToGL::g_pixel_types[pixel_type],          // (input) type
		pixels);                            // input data
}
void Texture2D::SetMinFilter(std::uint32_t filter) {
	assert(filter >= 0 && filter < Filter::NUM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertToGL::g_texture_filters[filter]);
}
void Texture2D::SetMagFilter(std::uint32_t filter) {
	assert(filter >= 0 && filter < Filter::NUM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertToGL::g_texture_filters[filter]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertToGL::g_texture_filters[filter]);
}
void Texture2D::SetWrapping(std::uint32_t wrap_mode) {
	assert(wrap_mode >= 0 && wrap_mode < Wrapping::NUM);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertToGL::g_texture_wrapping[wrap_mode]);
}
void Texture2D::SetBorderColor(const Color& color) {
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color.a);
}
}