#include "glw/font.h"
#include "glw/draw_commands.h"
#include "glw/matrix_stack.h"
#include "engine/utility/matrix4.h"
#include "engine/utility/logger.h"

#include <algorithm>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H


#ifdef WIN32
#include <Windows.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace OGL {
	bool Font::Load(const std::string& fontname, std::uint32_t height) {
		// initialize if necessary
		FT_Error error;
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			LOG_CRITICAL("Could not initialize freetype library");
			return false;
		}

		FT_Face face;
		if ((error = FT_New_Face(ft, fontname.c_str(), 0, &face))) {
			LOG_CRITICAL("Could not open font: " + fontname);
			if (error == FT_Err_Unknown_File_Format)
				LOG_CRITICAL("Unknown File Format");
			else
				LOG_CRITICAL("FT_Error: " + error);
			return false;
		}

		if (FT_Set_Pixel_Sizes(face, 0, height)) {
			LOG_CRITICAL("Could not set font size: " + fontname);
			return false;
		}

		_height = height;
		_bearingY = 0;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		for (int c = 0; c < 128; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				LOG_CRITICAL("Could not load character: " + std::to_string(c));
				continue;
			}

			unsigned texture_id;
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// store glyph

			Glyph glyph = {
				texture_id,
				{(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
				{face->glyph->bitmap_left, face->glyph->bitmap_top},
				(int)face->glyph->advance.x
			};

			_bearingY = std::max(glyph.bearing[1], (int)_bearingY);

			_glyphs.emplace_back(glyph);
		}


		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		return true;
	}
	void Font::RenderText(const std::string& text, float x, float y, const Color& color, float scale) const {
		static int mvp_loc = Draw::_fnt_shd.DeclUniform("in_mvp");
		static int tex_loc = Draw::_fnt_shd.DeclUniform("in_samp");
		static int col_loc = Draw::_fnt_shd.DeclUniform("in_color");

		Draw::_fnt_shd.Bind();
		Draw::_fnt_shd.SetUniformFloat4v(col_loc, &color.r);

		glActiveTexture(GL_TEXTURE0);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
		for (auto c = text.begin(); c != text.end(); c++) {
			const Glyph& g = _glyphs[*c];

			float pos[2] = {
				x + g.bearing[0] * scale,
				y + (g.bearing[1] - g.size[1]) * scale
				// y + (g.size[1]-g.bearing[1]) * scale
			};

			float dim[2] = {
				g.size[0] * scale,           // width
				g.size[1] * scale            // height
			};

			MatrixStack::PushModelView();
			MatrixStack::TranslateModelView(pos[0] + 0.5f*dim[0], pos[1] + 0.5f*dim[1], 0);
			MatrixStack::ScaleModelView(dim[0], dim[1], 1.0f);

			Mat4 mvp = MatrixStack::GetModelViewProjection();

			glBindTexture(GL_TEXTURE_2D, g.tex_id); 

			Draw::_fnt_shd.SetUniformInt(tex_loc, 0);
			Draw::_fnt_shd.SetUniformMatrix(mvp_loc, mvp.get());

			Draw::_quad_vbo.DrawTriangles(Draw::_fnt_shd);
			

			MatrixStack::PopModelView();

			x += (g.advance >> 6) * scale;
		}
		
		glPopAttrib();
		glBindTexture(GL_TEXTURE_2D, 0);
		Draw::_fnt_shd.Unbind();
	}
	int Font::GetAdvance(int c) const {
		return _glyphs[c].advance >> 6;
	}
	int Font::GetWidth(int c) const {
		return _glyphs[c].size[0];
	}
	int Font::GetHeight(int c) const {
		return _glyphs[c].size[1];
	}
	int Font::GetBearingX(int c) const {
		return _glyphs[c].bearing[0];
	}
	int Font::GetBearingY(int c) const {
		return _glyphs[c].bearing[1];
	}
	int Font::GetHeight() const {
		return _height;
	}
	int Font::GetBearingY() const {
		return _bearingY;
	}
	void Font::Destroy() {
		for (auto& g : _glyphs) {
			glDeleteTextures(1, &g.tex_id);
		}
	}
}