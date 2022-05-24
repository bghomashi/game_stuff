#ifndef __OGL_VERTEX_BUFFER__
#define __OGL_VERTEX_BUFFER__

#include <vector>
#include <unordered_map>
#include <cstdint>
#include "glw/shader.h"

namespace OGL {
	class VertexBuffer {
		std::uint32_t  _vbo_id, _vao_id;
		int _num_elements;
		int _element_size;

		std::unordered_map<int, int> _sizes;
		std::unordered_map<int, int> _offsets;
	public:
		void Create();
		void Create(int n, const void* data, const std::unordered_map<int, int>& attribute_sizes, const std::unordered_map<int, int>& attribute_offsets);
		void Data(int n, const void* data, const std::unordered_map<int, int>& attribute_sizes, const std::unordered_map<int, int>& attribute_offsets);
		void Destroy();

		void DrawTriangleFans(const Shader& shader, std::uint32_t start = 0, std::uint32_t count = 0) const;
		void DrawTriangles(const Shader& shader, std::uint32_t start = 0, std::uint32_t count = 0) const;
		void DrawLines(const Shader& shader, std::uint32_t start = 0, std::uint32_t count = 0) const;
		void DrawLineStrips(const Shader& shader, std::uint32_t start = 0, std::uint32_t count = 0) const;
		void DrawPoints(const Shader& shader, std::uint32_t start = 0, std::uint32_t count = 0) const;

		int GetEleSize() const;
		int GetNumEle() const;
	};
}
#endif