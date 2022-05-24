#ifndef __OGL_DRAW_COMMANDS__
#define __OGL_DRAW_COMMANDS__

#include "glw/vertex_buffer.h"
#include "glw/shader.h"
#include "glw/texture2d.h"
#include "glw/color.h"
#include "glw/font.h"
#include "utility/vector2.h"
#include <string>

namespace OGL {
	class Draw {
		friend Font;

		static Shader _col_shd, _tex_shd, _tex_col_shd, _fnt_shd, _part_tex_shd;
		static VertexBuffer _line_vbo, _quad_vbo, _circle_vbo;
	public:
		static bool Initialize();

		// draw functions
		static void SquareFilled(Vec2 center, Vec2 size, Color color);
		static void SquareFilled(Vec2 center, float angle, Vec2 size, Color color);
		static void Square(Vec2 center, Vec2 size, Color color);
		static void Square(Vec2 center, float angle, Vec2 size, Color color);
		static void SquareTexture(const Vec2& center, const Vec2& size, const Texture2D& tex);
		static void SquarePartialTexture(const Vec2& center, const Vec2& size, const Vec2& from_pixel, const Vec2& size_pixel, const Texture2D& tex, int alpha_mask = 0);
		static void SquareTexture(const Vec2& center, const Vec2& size, const Texture2D& tex, const Color& color);
		static void CircleFilled(Vec2 center, float radius, Color color);
		static void Circle(Vec2 center, float radius, Color color);
		static void EllipseFilled(Vec2 center, float h_radius, float v_radius, Color color);
		static void EllipseFilled(Vec2 center, float angle, float h_radius, float v_radius, Color color);
		static void Ellipse(Vec2 center, float h_radius, float v_radius, Color color);
		static void Ellipse(Vec2 center, float angle, float h_radius, float v_radius, Color color);
		static void Line(const Vec2& a, const Vec2& b, const Color& color);
	};
}

#endif