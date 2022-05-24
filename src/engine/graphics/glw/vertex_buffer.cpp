#include "glw/vertex_buffer.h"
#include "utility/logger.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OGL {
	int VertexBuffer::GetEleSize() const {
		return _element_size;
	}
	int VertexBuffer::GetNumEle() const {
		return _num_elements;
	}
	void VertexBuffer::Create() {
		glGenVertexArrays(1, &_vao_id);
		glBindVertexArray(_vao_id);
		
		glGenBuffers(1, &_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::Create(
		int n, 
		const void* data, 
		const std::unordered_map<int, int>& attribute_sizes, 
		const std::unordered_map<int, int>& attribute_offsets) {
		_num_elements = n;
		_sizes = attribute_sizes;
		_offsets = attribute_offsets;
		_element_size = 0;
		
		for (auto& a : attribute_sizes)
			_element_size += a.second * sizeof(float);				// this assumes all attributes are floats
		
		glGenVertexArrays(1, &_vao_id);
		glBindVertexArray(_vao_id);

		glGenBuffers(1, &_vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
		glBufferData(GL_ARRAY_BUFFER, _element_size * n, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::Data(
		int n, 
		const void* data,
		const std::unordered_map<int, int>& attribute_sizes,
		const std::unordered_map<int, int>& attribute_offsets) {
		_num_elements = n;
		_sizes = attribute_sizes;
		_offsets = attribute_offsets;
		_element_size = 0;

		for (auto& a : attribute_sizes)
			_element_size += a.second * sizeof(float);				// this assumes all attributes are floats

		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);
		glBufferData(GL_ARRAY_BUFFER, _element_size * n, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::Destroy() {
		glDeleteBuffers(1, &_vbo_id);
	}
	void VertexBuffer::DrawTriangleFans(const Shader& shader, std::uint32_t start, std::uint32_t count) const {
		glBindVertexArray(_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);

		// enable attributes
		shader.ForEachAttribute([=](int attrib, int id) {
			glEnableVertexAttribArray(id);
			// set pointers
			glVertexAttribPointer(id,
				_sizes.at(attrib),
				GL_FLOAT, false,
				_element_size,
				(void*)(_offsets.at(attrib) * sizeof(float)));
		});

		// draw
		glDrawArrays(
			GL_TRIANGLE_FAN, 
			start, 
			(count == 0 ? _num_elements : count) - start);
		// disable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glDisableVertexAttribArray(id);
		});

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void VertexBuffer::DrawTriangles(const Shader& shader, std::uint32_t start, std::uint32_t count) const {
		glBindVertexArray(_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);

		// enable attributes
		shader.ForEachAttribute([=](int attrib, int id) {
			glEnableVertexAttribArray(id);
			// set pointers
			glVertexAttribPointer(id,
				_sizes.at(attrib),
				GL_FLOAT, false,
				_element_size,
				(void*)(_offsets.at(attrib) * sizeof(float)));
		});

		// draw
		glDrawArrays(
			GL_TRIANGLES, 
			start, 
			(count == 0 ? _num_elements : count) - start);
		// disable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glDisableVertexAttribArray(id);
		});

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void VertexBuffer::DrawLines(const Shader& shader, std::uint32_t start, std::uint32_t count) const {
		glBindVertexArray(_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);

		shader.ForEachAttribute([=](int attrib, int id) {
			// enable attributes
			glEnableVertexAttribArray(id);
			// set pointers
			glVertexAttribPointer(id,
				_sizes.at(attrib),
				GL_FLOAT, false,
				_element_size,
				(void*)(_offsets.at(attrib) * sizeof(float)));
		});

		// draw
		glDrawArrays(
			GL_LINES, 
			start, 
			(count == 0 ? _num_elements : count) - start);
		// disable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glDisableVertexAttribArray(id);
		});

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void VertexBuffer::DrawLineStrips(const Shader& shader, std::uint32_t start, std::uint32_t count) const {
		glBindVertexArray(_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);

		// enable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glEnableVertexAttribArray(id);
			});
		// set pointers
		shader.ForEachAttribute([=](int attrib, int id) {
			glVertexAttribPointer(id,
				_sizes.at(attrib),
				GL_FLOAT, false,
				_element_size,
				(void*)(_offsets.at(attrib) * sizeof(float)));
			});

		// draw
		glDrawArrays(
			GL_LINE_STRIP, 
			start, 
			(count == 0 ? _num_elements : count) - start);
		// disable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glDisableVertexAttribArray(id);
			});

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	void VertexBuffer::DrawPoints(const Shader& shader, std::uint32_t start, std::uint32_t count) const {
		glBindVertexArray(_vao_id);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo_id);

		// enable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glEnableVertexAttribArray(id);
			});
		// set pointers
		shader.ForEachAttribute([=](int attrib, int id) {
			glVertexAttribPointer(id,
				_sizes.at(attrib),
				GL_FLOAT, false,
				_element_size,
				(void*)(_offsets.at(attrib) * sizeof(float)));
			});

		// draw
		glDrawArrays(
			GL_POINTS,
			start,
			(count == 0 ? _num_elements : count) - start);
		// disable attributes
		shader.ForEachAttribute([](int attrib, int id) {
			glDisableVertexAttribArray(id);
		});

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
