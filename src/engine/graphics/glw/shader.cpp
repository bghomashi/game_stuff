#include "glw/shader.h"
#include "engine/utility/logger.h"

#ifdef WIN32
#include <Windows.h>
#endif
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace OGL {
	bool Shader::Create(const std::string& v_src, const std::string& f_src)
	{
		const char* src;
		int success;
		char infoLog[512];

		// create vertex shader
		src = v_src.c_str();
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &src, NULL);
		glCompileShader(vertex);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			LOG_CRITICAL("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
			LOG_CRITICAL(infoLog);
			return false;
		};
		// create fragment shader
		src = f_src.c_str();
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &src, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (success != GL_TRUE) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			LOG_CRITICAL("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
			LOG_CRITICAL(infoLog);
			return false;
		};
		// create shader program
		GLuint id = glCreateProgram();
		glAttachShader(id, vertex);
		glAttachShader(id, fragment);
		glLinkProgram(id);

		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (success != GL_TRUE) {
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			LOG_CRITICAL("ERROR::SHADER::PROGRAM::LINKING_FAILED");
			LOG_CRITICAL(infoLog);
			return false;
		}
		// free vertex/fragment shader
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		_shd_id = id;
		return true;
	}
	void Shader::Bind() const
	{
		glUseProgram(_shd_id);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
	void Shader::DeclAttribute(const std::string& name, int type)
	{
		GLint loc = glGetAttribLocation(_shd_id, name.c_str());
		if (loc != -1)
			_attributes.push_back(std::make_pair(type, loc));
	}
	int Shader::DeclUniform(const std::string& name) const {
		GLint loc = glGetUniformLocation(_shd_id, name.c_str());
		//if (loc != -1)
		//	_uniforms.insert(std::make_pair(name, loc));
		return loc;
	}
	void Shader::SetUniformInt(int loc, int value) const {
		glUniform1i(loc, value);
	}
	void Shader::SetUniformUInt(int loc, int value) const {
		glUniform1ui(loc, value);
	}
	void Shader::SetUniformFloat(int loc, float value) const {
		glUniform1f(loc, value);
	}
	void Shader::SetUniformFloatv(int loc, const float* value, size_t count) const {
		glUniform1fv(loc, count, value);
	}
	void Shader::SetUniformFloat2v(int loc, const float* value, size_t count) const {
		glUniform2fv(loc, count, value);
	}
	void Shader::SetUniformFloat3v(int loc, const float* value, size_t count) const {
		glUniform3fv(loc, count, value);
	}
	void Shader::SetUniformFloat4v(int loc, const float* value, size_t count) const {
		glUniform4fv(loc, count, value);
	}
	void Shader::SetUniformMatrix(int loc, const float* m) const {
		glUniformMatrix4fv(loc, 1, false, m);
	}
	void Shader::ForEachAttribute(std::function<void(int, int)> func) const {
		for (auto a : _attributes)
			func(a.first, a.second);
	}
}
