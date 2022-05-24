#include "glw/device.h"
#include "internal_graphics.h"

#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace OGL {
    void Device::Clear(std::uint32_t bits) {
        glClear(ConvertToGL::g_clear_bits[bits]);
    }
    void Device::SetClearColor(const Color& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }
    void Device::SetClearDepth(float d) {
        glClearDepth(d);
    }
    void Device::SetDepthFunc(std::uint32_t func) {
        glDepthFunc(ConvertToGL::g_depth_test[func]);
    }
    void Device::SetDepthMask(bool flag) {
        glDepthMask(flag);
    }
    void Device::SetStencilMask(bool flag) {
        glStencilMask(flag);
    }
    void Device::SetDepthTest(bool flag) {
        if (flag)
            glEnable(GL_DEPTH_TEST);
        else 
            glDisable(GL_DEPTH_TEST);
    }
    void Device::SetStenciltest(bool flag) {
        if (flag)
            glEnable(GL_STENCIL_TEST);
        else
            glDisable(GL_STENCIL_TEST);
    }

    void Device::SetBlending(bool flag) {
        // std::cout << "called " << flag << std::endl;
        if (flag)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }
    void Device::SetBlendFunc(std::uint32_t src, std::uint32_t dst) {
        assert(src < BlendFunc::NUM);
        assert(dst < BlendFunc::NUM);
        glBlendFunc(ConvertToGL::g_blend_func[src], ConvertToGL::g_blend_func[dst]);
    }
	void Device::SetBlendEqua(unsigned equa) {
		assert(equa < BlendEquation::NUM);

		glBlendEquation(ConvertToGL::g_blend_eq[equa]);

	}

	void Device::SetLineStipple(int factor, std::uint16_t pattern) {
		glLineStipple(factor, pattern);
	}
	void Device::SetLineWidth(unsigned width) {
		glLineWidth((float)width);
	}
	void Device::SetPointSize(unsigned size) {
		glPointSize((float)size);
	}
	void Device::SetScissor(bool flag) {
		if (flag)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
	}
	void Device::Scissor(int x, int y, std::uint32_t width, std::uint32_t height) {
		glScissor(x, y, width, height);
	}

	void Device::SetViewport(int x, int y, std::uint32_t width, std::uint32_t height) {
		glViewport(x, y, width, height);
	}
	void Device::GetViewport(int& x, int& y, std::uint32_t& width, std::uint32_t& height) {
		int buff[4];
		glGetIntegerv(GL_VIEWPORT, buff);
		x = buff[0];
		y = buff[1];
		width = buff[2];
		height = buff[3];
	}
    void Device::SetSmoothLine(bool flag) {
        if (flag) {
            glEnable( GL_LINE_SMOOTH );
            glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
        } else {
            glDisable( GL_LINE_SMOOTH );
        }
    }
    void Device::SetMultisampling(bool flag) {
        if (flag) {
            glEnable(GL_MULTISAMPLE);
        } else {
            glDisable(GL_MULTISAMPLE);
        }
    }
}