#ifndef __OGL_MATRIX_STACK__
#define __OGL_MATRIX_STACK__

#include <stack>
#include "utility/matrix4.h"

namespace OGL {
	class MatrixStack {
		static std::stack<Mat4> s_projection_stack;
		static std::stack<Mat4> s_modelview_stack;
	public:
		static void MultiProjection(const Mat4& m);
		static void MultiModelView(const Mat4& m);
		static void PushProjection();
		static void PushModelView();
		static void PopProjection();
		static void PopModelView();
		static void LoadProjection(const Mat4& m);
		static void LoadModelView(const Mat4& m);
		static const Mat4& GetProjection();
		static const Mat4& GetModelView();
		static const Mat4 GetModelViewProjection();

		static void OrthoProjection(float left, float right, float bottom, float top, float near = -1.0, float far = 1.0);
		static void ScaleProjection(float x, float y, float z);
		static void ScaleModelView(float x, float y, float z);
		static void TranslateProjection(float x, float y, float z);
		static void TranslateModelView(float x, float y, float z);
		static void RotateProjection(float a, float x, float y, float z);
		static void RotateModelView(float a, float x, float y, float z);
	};
}
#endif