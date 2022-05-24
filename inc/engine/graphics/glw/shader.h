#ifndef __OGL_SHADER__
#define __OGL_SHADER__

#include <vector>
#include <string>
#include <functional>

namespace OGL {
	struct Attribute {
		enum {
			Position = 0,
			Color,
			Normal,
			Texture,
			Tile,

			Num,
		};
	};

	class Shader {
		friend class VertexBuffer;

		std::uint32_t _shd_id;
		std::vector<std::pair<int, int>> _attributes;
		// std::unordered_map<std::string, int> _uniforms;

		void ForEachAttribute(std::function<void(int, int)> func) const;
	public:
		bool Create(const std::string& v_src, const std::string& f_src);
		void DeclAttribute(const std::string& name, int type);
		int DeclUniform(const std::string& name) const;
		void Bind() const;
		void Unbind() const;

		void SetUniformInt(int loc, int value) const;
		void SetUniformUInt(int loc, int value) const;
		void SetUniformFloat(int loc, float value) const;
		void SetUniformFloatv(int loc, const float* value, size_t count = 1) const;
		void SetUniformFloat2v(int loc, const float* value, size_t count = 1) const;
		void SetUniformFloat3v(int loc, const float* value, size_t count = 1) const;
		void SetUniformFloat4v(int loc, const float* value, size_t count = 1) const;
		void SetUniformMatrix(int loc, const float* m) const;
	};

}
#endif