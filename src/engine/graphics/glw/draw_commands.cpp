
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <cassert>
#include <iostream>

#include <GL/gl.h>

#include "glw/draw_commands.h"
#include "glw/vertex_buffer.h"
#include "glw/shader.h"
#include "glw/matrix_stack.h"
#include "glw/texture2d.h"
#include "utility/matrix4.h"
#include "utility/vector2.h"
#include "utility/logger.h"

namespace OGL {
	Shader Draw::_col_shd, Draw::_tex_shd, Draw::_tex_col_shd, Draw::_fnt_shd, Draw::_part_tex_shd;
	VertexBuffer Draw::_line_vbo, Draw::_quad_vbo, Draw::_circle_vbo;

	VertexBuffer InitCircle() {
		const unsigned sides = 30;
		float rad = 0;
		std::vector<float> data;
		data.reserve((sides + 1) * 4);
		data.push_back(0.0f);
		data.push_back(0.0f);
		data.push_back(0.5f);
		data.push_back(0.5f);

		for (int i = 0; i <= sides; i++) {
			rad = i*(2.0f*M_PI)/sides;
			data.push_back(cos(rad)*1.f);
			data.push_back(sin(rad)*1.f);
			data.push_back(cos(rad)*0.5f + 0.5f);
			data.push_back(sin(rad)*0.5f + 0.5f);
		}


		VertexBuffer vbo;
		vbo.Create(
			(sides + 2), &data[0],
			{{Attribute::Position, 2}, {Attribute::Texture, 2}},
			{{Attribute::Position, 0}, {Attribute::Texture, 2}});

		return vbo;
	};
	VertexBuffer InitQuad() {
		float data[] = {
			-0.5, 0.5, 0.0, 0.0,
			-0.5,-0.5, 0.0, 1.0,
			0.5,-0.5, 1.0, 1.0,

			0.5,-0.5, 1.0, 1.0,
			0.5, 0.5, 1.0, 0.0,
			-0.5, 0.5, 0.0, 0.0,
		};

		VertexBuffer vbo;
		vbo.Create(
			6, data,
			{ {Attribute::Position, 2}, {Attribute::Texture, 2} },
			{ {Attribute::Position, 0}, {Attribute::Texture, 2} });

		return vbo;
	};
	VertexBuffer InitLine() {
		float data[] = {
			0.0, 0.0, 
			1.0, 0.0, 
		};

		VertexBuffer vbo;
		vbo.Create(
			2, data,
			{ {Attribute::Position, 2} },
			{ {Attribute::Position, 0} });

		return vbo;
	};
	bool InitPartialTextureQuadShader(Shader& shader) {
		const char* vshader =
			"#version 150\n"
			"attribute vec2 in_pos;\n"
			"attribute vec2 in_tex;\n"
			"uniform mat4 in_mvp;\n"
			"uniform sampler2D in_samp;\n"
			"uniform vec2 in_from_pixel;\n"
			"uniform vec2 in_size_pixel;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"   vec2 tex_size = textureSize(in_samp,0);\n"
			"   vec2 from_pixel = in_from_pixel / tex_size;\n"
			"   vec2 size_pixel = in_size_pixel / tex_size;\n"
			"	gl_Position = in_mvp * vec4(in_pos.xy, 0, 1);\n"
			"	out_tex = from_pixel + in_tex*size_pixel;\n"
			"}\n";

		const char* fshader =
			"#version 150\n"
			"uniform sampler2D in_samp;\n"
			"uniform int in_alpha_mask;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_FragColor = texture(in_samp, out_tex);\n"
			"   if (gl_FragColor.a <= 0.0001)\n"
			"        discard;"
			"}\n";

		if (!shader.Create(vshader, fshader)) {
			LOG_CRITICAL("InitTextureQuadShader() failed");
			return false;
		}

		shader.DeclAttribute("in_pos", Attribute::Position);
		shader.DeclAttribute("in_tex", Attribute::Texture);

		return true;
	}
	bool InitTextureColorQuadShader(Shader& shader) {
		const char* vshader =
			"#version 150\n"
			"attribute vec2 in_pos;\n"
			"attribute vec2 in_tex;\n"
			"uniform mat4 in_mvp;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_Position = in_mvp * vec4(in_pos.xy, 0, 1);\n"
			"	out_tex = in_tex;\n"
			"}\n";

		const char* fshader =
			"#version 150\n"
			"uniform sampler2D in_samp;\n"
			"uniform vec4 in_col;"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_FragColor = in_col*texture(in_samp, out_tex);\n"
			"}\n";

		if (!shader.Create(vshader, fshader)) {
			LOG_CRITICAL("InitTextureQuadShader() failed");
			return false;
		}

		shader.DeclAttribute("in_pos", Attribute::Position);
		shader.DeclAttribute("in_tex", Attribute::Texture);

		return true;
	}
	bool InitTextureQuadShader(Shader& shader) {
		const char* vshader =
			"#version 150\n"
			"attribute vec2 in_pos;\n"
			"attribute vec2 in_tex;\n"
			"uniform mat4 in_mvp;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_Position = in_mvp * vec4(in_pos.xy, 0, 1);\n"
			"	out_tex = in_tex;\n"
			"}\n";

		const char* fshader =
			"#version 150\n"
			"uniform sampler2D in_samp;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_FragColor = texture(in_samp, out_tex);\n"
			"}\n";

		if (!shader.Create(vshader, fshader)) {
			LOG_CRITICAL("InitTextureQuadShader() failed");
			return false;
		}

		shader.DeclAttribute("in_pos", Attribute::Position);
		shader.DeclAttribute("in_tex", Attribute::Texture);

		return true;
	}
	bool InitColorQuadShader(Shader& shader) {
		const char* vshader =
			"#version 150\n"
			"attribute vec2 in_pos;\n"
			"uniform mat4 in_mvp;\n"

			"void main(void) {\n"
			"	gl_Position = in_mvp * vec4(in_pos.xy, 0, 1);\n"
			"}\n";

		const char* fshader =
			"#version 150\n"
			"uniform vec4 in_color;\n"

			"void main(void) {\n"
			"	gl_FragColor = in_color;\n"
			"}\n";

		if (!shader.Create(vshader, fshader)) {
			LOG_CRITICAL("InitColorQuadShader() failed");
			return false;
		}

		shader.DeclAttribute("in_pos", Attribute::Position);

		return true;
	}
	bool InitFontShader(Shader& shader) {
		const char* vshader = 
			"#version 150\n"
			"attribute vec2 in_pos;\n"
			"attribute vec2 in_tex;\n"
			"uniform mat4 in_mvp;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	gl_Position = in_mvp * vec4(in_pos.xy, 0, 1);\n"
			"	out_tex = in_tex;\n"
			"}\n";

		const char* fshader = 
			"#version 150\n"
			"uniform vec4 in_color;\n"
			"uniform sampler2D in_samp;\n"
			"varying vec2 out_tex;\n"

			"void main(void) {\n"
			"	vec4 glyph = vec4(1.0, 1.0, 1.0, texture(in_samp, out_tex).r);\n"
			"	gl_FragColor = in_color * glyph;\n"
			"}\n";


		if (!shader.Create(vshader, fshader)) {
			LOG_CRITICAL("InitFontShader() failed");
			return false;
		}

		shader.DeclAttribute("in_pos", Attribute::Position);
		shader.DeclAttribute("in_tex", Attribute::Texture);

		return true;
	}
	bool Draw::Initialize() {
		if (!InitColorQuadShader(_col_shd) ||
			!InitTextureQuadShader(_tex_shd) ||
			!InitTextureColorQuadShader(_tex_col_shd) ||
			!InitFontShader(_fnt_shd) || 
			!InitPartialTextureQuadShader(_part_tex_shd))
			return false;

		_line_vbo = InitLine();
		_quad_vbo = InitQuad();
		_circle_vbo = InitCircle();

		return true;
	}

	void Draw::SquareFilled(Vec2 position, Vec2 size, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_quad_vbo.DrawTriangles(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::SquareFilled(Vec2 position, float angle, Vec2 size, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::RotateModelView(angle, 0, 0, -1.f);
		MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_quad_vbo.DrawTriangles(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::Square(Vec2 position, Vec2 size, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_quad_vbo.DrawLineStrips(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::Square(Vec2 position, float angle, Vec2 size, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::RotateModelView(angle, 0, 0, -1.f);
		MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_quad_vbo.DrawLineStrips(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::SquareTexture(const Vec2& position, const Vec2& size, const Texture2D& tex) {
	 	static int mvp_loc = _tex_shd.DeclUniform("in_mvp");
	 	static int smp_loc = _tex_shd.DeclUniform("in_samp");

	 	MatrixStack::PushModelView();
	 	MatrixStack::TranslateModelView(position.x, position.y, 0);
	 	MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

	 	Mat4 mvp = MatrixStack::GetModelViewProjection();

	 	_tex_shd.Bind();
	 	tex.Bind(0);

	 	_tex_shd.SetUniformFloat(smp_loc, 0);
	 	_tex_shd.SetUniformMatrix(mvp_loc, mvp.get());

		// std::cout << (glIsEnabled(GL_BLEND) == GL_TRUE) << std::endl;
	 	_quad_vbo.DrawTriangles(_tex_shd);

	 	tex.Unbind(0);
	 	_tex_shd.Unbind();

	 	MatrixStack::PopModelView();
	 }
	void Draw::SquareTexture(const Vec2& position, const Vec2& size, const Texture2D& tex, const Color& color) {
	 	static int mvp_loc = _tex_col_shd.DeclUniform("in_mvp");
	 	static int smp_loc = _tex_col_shd.DeclUniform("in_samp");
	 	static int col_loc = _tex_col_shd.DeclUniform("in_col");

	 	MatrixStack::PushModelView();
	 	MatrixStack::TranslateModelView(position.x, position.y, 0);
	 	MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

	 	Mat4 mvp = MatrixStack::GetModelViewProjection();

	 	_tex_col_shd.Bind();
	 	tex.Bind(0);

	 	_tex_col_shd.SetUniformFloat(smp_loc, 0);
	 	_tex_col_shd.SetUniformMatrix(mvp_loc, mvp.get());
	 	_tex_col_shd.SetUniformFloat4v(col_loc, &color.r);


	 	_quad_vbo.DrawTriangles(_tex_col_shd);

	 	tex.Unbind(0);
	 	_tex_col_shd.Unbind();

	 	MatrixStack::PopModelView();
	 }

	void Draw::SquarePartialTexture(const Vec2& position, const Vec2& size, const Vec2& from_pixel, const Vec2& sizes_pixel,const Texture2D& tex, int alpha_mask) {
		static int mvp_loc = _part_tex_shd.DeclUniform("in_mvp");
		static int smp_loc = _part_tex_shd.DeclUniform("in_samp");
		static int fp_loc = _part_tex_shd.DeclUniform("in_from_pixel");
		static int sp_loc = _part_tex_shd.DeclUniform("in_size_pixel");
		static int msk_loc = _part_tex_shd.DeclUniform("in_alpha_mask");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(size.x, size.y, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();


		_part_tex_shd.Bind();
		tex.Bind(0);

		_part_tex_shd.SetUniformFloat(smp_loc, 0);
		_part_tex_shd.SetUniformMatrix(mvp_loc, mvp.get());
		_part_tex_shd.SetUniformFloat2v(fp_loc, &from_pixel.x, 1);
		_part_tex_shd.SetUniformFloat2v(sp_loc, &sizes_pixel.x, 1);
		_part_tex_shd.SetUniformInt(msk_loc, alpha_mask);

		_quad_vbo.DrawTriangles(_part_tex_shd);

		tex.Unbind(0);
		_part_tex_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::CircleFilled(Vec2 position, float radius, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(radius, radius, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawTriangleFans(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::Circle(Vec2 position, float radius, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(radius, radius, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawLineStrips(_col_shd, 1);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::EllipseFilled(Vec2 position, float hr, float vr, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(hr, vr, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawTriangleFans(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::EllipseFilled(Vec2 position, float angle, float hr, float vr, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::RotateModelView(angle, 0, 0, -1.f);
		MatrixStack::ScaleModelView(hr, vr, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawTriangleFans(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::Ellipse(Vec2 position, float angle, float hr, float vr, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::RotateModelView(angle, 0, 0, -1.f);
		MatrixStack::ScaleModelView(hr, vr, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawLineStrips(_col_shd, 1);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
	void Draw::Ellipse(Vec2 position, float hr, float vr, Color color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(position.x, position.y, 0);
		MatrixStack::ScaleModelView(hr, vr, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_circle_vbo.DrawLineStrips(_col_shd, 1);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}

	void Draw::Line(const Vec2& a, const Vec2& b, const Color& color) {
		static int mvp_loc = _col_shd.DeclUniform("in_mvp");
		static int col_loc = _col_shd.DeclUniform("in_color");

		Vec2 dis = b - a;
		Vec2 pos = a;

		MatrixStack::PushModelView();
		MatrixStack::TranslateModelView(pos.x, pos.y, 0);
		MatrixStack::RotateModelView(atan2(dis.y, dis.x), 0, 0, -1);
		MatrixStack::ScaleModelView(dis.length(), 1.0f, 1.0f);

		Mat4 mvp = MatrixStack::GetModelViewProjection();

		_col_shd.Bind();
		_col_shd.SetUniformFloat4v(col_loc, &color.r);
		_col_shd.SetUniformMatrix(mvp_loc, mvp.get());

		_line_vbo.DrawLineStrips(_col_shd);

		_col_shd.Unbind();

		MatrixStack::PopModelView();
	}
}