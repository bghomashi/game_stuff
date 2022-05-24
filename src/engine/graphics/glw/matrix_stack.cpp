#include "glw/matrix_stack.h"


namespace OGL {
	void MatrixStack::OrthoProjection(float left, float right, float bottom, float top, float near, float far) {
		auto& m = s_projection_stack.top();
		m.Ortho(left, right, bottom, top, near, far);
	}
	void MatrixStack::ScaleProjection(float x, float y, float z) {
		auto& m = s_projection_stack.top();
		auto* ptr = m.get();
		for (int i = 0; i < 4; i++) {
			ptr[0 + i] *= x;
			ptr[4 + i] *= y;
			ptr[8 + i] *= z;
		}
	}
	void MatrixStack::TranslateProjection(float x, float y, float z) {
		auto& m = s_projection_stack.top();
		auto* ptr = m.get();
		float tx = ptr[0]*x + ptr[4]*y + ptr[ 8]*z + ptr[12];
		float ty = ptr[1]*x + ptr[5]*y + ptr[ 9]*z + ptr[13];
		float tz = ptr[2]*x + ptr[6]*y + ptr[10]*z + ptr[14];
		float tw = ptr[3]*x + ptr[7]*y + ptr[11]*z + ptr[15];

		ptr[12] = tx;
		ptr[13] = ty;
		ptr[14] = tz;
		ptr[15] = tw;
	}
	void MatrixStack::ScaleModelView(float x, float y, float z) {
		auto& m = s_modelview_stack.top();
		auto* ptr = m.get();
		for (int i = 0; i < 4; i++) {
			ptr[0 + i] *= x;
			ptr[4 + i] *= y;
			ptr[8 + i] *= z;
		}
	}
	void MatrixStack::TranslateModelView(float x, float y, float z) {
		auto& m = s_modelview_stack.top();
		auto* ptr = m.get();
		float tx = ptr[0]*x + ptr[4]*y + ptr[ 8]*z + ptr[12];
		float ty = ptr[1]*x + ptr[5]*y + ptr[ 9]*z + ptr[13];
		float tz = ptr[2]*x + ptr[6]*y + ptr[10]*z + ptr[14];
		float tw = ptr[3]*x + ptr[7]*y + ptr[11]*z + ptr[15];

		ptr[12] = tx;
		ptr[13] = ty;
		ptr[14] = tz;
		ptr[15] = tw;
	}
	void MatrixStack::RotateProjection(float a, float x, float y, float z) {
		auto& m = s_projection_stack.top();
		m.Rotate(a, {x, y, z});				// this might not be in the right order?
	}
	void MatrixStack::RotateModelView(float a, float x, float y, float z) {
		auto& m = s_modelview_stack.top();
		m.Rotate(a, {x, y, z});				// this might not be in the right order?
	}
	
	void MatrixStack::MultiProjection(const Mat4& m) {
		s_projection_stack.top() *= m;
	}
	void MatrixStack::MultiModelView(const Mat4& m) {
		s_modelview_stack.top() *= m;
	}
	void MatrixStack::PushProjection() {
		s_projection_stack.push(s_projection_stack.top());
	}
	void MatrixStack::PushModelView() {
		s_modelview_stack.push(s_modelview_stack.top());
	}
	void MatrixStack::PopProjection() {
		s_projection_stack.pop();
	}
	void MatrixStack::PopModelView() {
		s_modelview_stack.pop();
	}
	void MatrixStack::LoadProjection(const Mat4& m) {
		s_projection_stack.top() = m;
	}
	void MatrixStack::LoadModelView(const Mat4& m) {
		s_modelview_stack.top() = m;
	}
	const Mat4& MatrixStack::GetProjection() {
		return s_projection_stack.top();
	}
	const Mat4& MatrixStack::GetModelView() {
		return s_modelview_stack.top();
	}
	const Mat4 MatrixStack::GetModelViewProjection() {
		return s_projection_stack.top() * s_modelview_stack.top();
	}

	std::stack<Mat4> InitMatrixStack() {
		std::stack<Mat4> s;
		s.push(Mat4());
		return s;
	}

	std::stack<Mat4> MatrixStack::s_projection_stack = InitMatrixStack();
	std::stack<Mat4> MatrixStack::s_modelview_stack = InitMatrixStack();
}