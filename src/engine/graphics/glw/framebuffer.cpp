#include "glw/framebuffer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include "internal_graphics.h"

namespace OGL {
	FrameBuffer FrameBuffer::GetDefault() {
		FrameBuffer def;
		def._frm_id = 0;
		return def;
	}
	void FrameBuffer::Create() {
		glGenFramebuffers(1, &_frm_id);
	}
	void FrameBuffer::Destroy() {
		glDeleteFramebuffers(1, &_frm_id);
		_frm_id = 0;
	}



	bool FrameBuffer::IsComplete() const {
		if (_frm_id == 0) return true;
		int cur;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &cur);
		glBindFramebuffer(GL_FRAMEBUFFER, _frm_id);
		bool status = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, cur);
		return status;
	}

	void FrameBuffer::Bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, _frm_id);

	}

	void FrameBuffer::Unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::AttachColorBuffer(Texture2D& texture) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			texture._tex_id,
			0);
	}



	void RenderBuffer::Create() {
		glGenRenderbuffers(1, &_rnd_id);
	}
	void RenderBuffer::Destroy() {
		glDeleteRenderbuffers(1, &_rnd_id);
		_rnd_id = 0;
	}
	void RenderBuffer::Bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, _rnd_id);
	}

	void RenderBuffer::Storage(std::uint32_t width, std::uint32_t height, std::uint32_t format) {
		assert(format >= 0 && format < InternalFormat::NUM);
		int cur;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &cur);
		glBindRenderbuffer(GL_RENDERBUFFER, _rnd_id);
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			ConvertToGL::g_internal_formats[format],
			width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, cur);
	}
}