#include "Shader.h"
#include "Logger.h"

#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Ember {
	static uint32_t current_shader_binded = 0;

	Shader::Shader(const std::string& file_path) {
		Init(file_path);
	}

	Shader::~Shader() {
		glDeleteProgram(shader_id);
	}

	void Shader::Bind() {
		if (shader_id != current_shader_binded) {
			glUseProgram(shader_id);
			current_shader_binded = shader_id;
		}
	}

	void Shader::UnBind() {
		glUseProgram(0);
		current_shader_binded = 0;
	}

	void Shader::Init(const std::string& file_path) {
		shader_id = CreateShader(ParseShader(file_path));
	}

	uint32_t Shader::CompileShader(const std::string& source, uint32_t type) {
		uint32_t id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)malloc(length * sizeof(char));
			glGetShaderInfoLog(id, length, &length, message);
			EMBER_LOG_ERROR("Shader failed to load: %s", message);

			glDeleteShader(id);
			return 0;
		}

		return id;
	}

	ShaderSources Shader::ParseShader(const std::string& file_path) {
		std::ifstream stream(file_path);

		enum class ShaderType {
			NONE = -1, VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER, GEOMETRY = GL_GEOMETRY_SHADER, TESS_EVAL = GL_TESS_EVALUATION_SHADER, TESS_CONTROL = GL_TESS_CONTROL_SHADER
		};

		ShaderType type = ShaderType::NONE;
		std::string line;

		if (!stream.is_open()) {
			EMBER_LOG_ERROR("Failed to load asset shader '%s'.", file_path.c_str());
			return ShaderSources();
		}
		else
			EMBER_LOG_GOOD("Asset shader '%s' loaded.", file_path.c_str());

		ShaderSources ss;
		while (getline(stream, line)) {
			if (line.find("#shader") != std::string::npos) {
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::VERTEX;
				else if (line.find("fragment") != std::string::npos)
					type = ShaderType::FRAGMENT;
				else if (line.find("geometry") != std::string::npos)
					type = ShaderType::GEOMETRY;
				else if (line.find("tess-control") != std::string::npos)
					type = ShaderType::TESS_CONTROL;
				else if (line.find("tess-eval") != std::string::npos)
					type = ShaderType::TESS_EVAL;
			}
			else {
				ss[(uint32_t)type] << line << '\n';
			}
		}

		stream.close();
		return ss;
	}

	uint32_t Shader::CreateShader(const ShaderSources& shader_sources) {
		uint32_t program = glCreateProgram();

		for (auto& shader : shader_sources) {
			uint32_t s = CompileShader(shader.second.str(), shader.first);
			glAttachShader(program, s);
			glDeleteShader(s);
		}

		glLinkProgram(program);  
		glValidateProgram(program);

		return program;
	}

	uint32_t ProgramGetUniformLocation(uint32_t id, const std::string& name) {
		return (glGetUniformLocation(id, name.c_str()));
	}

	void ProgramSet1f(uint32_t id, const std::string& name, float value) {
		glProgramUniform1f(id, ProgramGetUniformLocation(id, name), value);
	}

	void ProgramSetMat4f(uint32_t id, const std::string& name, const glm::mat4& mat4) {
		glProgramUniformMatrix4fv(id, ProgramGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void ProgramSetVec3f(uint32_t id, const std::string& name, const glm::vec3& vec3) {
		glProgramUniform3f(id, ProgramGetUniformLocation(id, name), vec3.x, vec3.y, vec3.z);
	}

	void ProgramSetIntArray(uint32_t id, const std::string& name, int* array) {
		glProgramUniform1iv(id, ProgramGetUniformLocation(id, name), sizeof(array) / sizeof(int), array);
	}

	void Shader::Set1f(const std::string& name, float value) {
		ProgramSet1f(shader_id, name, value);
	}

	uint32_t Shader::GetUniformLocation(const std::string& name) {
		return (glGetUniformLocation(shader_id, name.c_str()));
	}

	void Shader::SetMat4f(const std::string& name, const glm::mat4& mat4) {
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void Shader::SetVec3f(const std::string& name, const glm::vec3& vec3) {
		glUniform3f(GetUniformLocation(name), vec3.x, vec3.y, vec3.z);
	}

	void Shader::SetIntArray(const std::string& name, int* array, uint32_t size) {
		glUniform1iv(GetUniformLocation(name), size, array);
	}

	std::vector<std::string> GetUniformNames(uint32_t id) {
		GLint i;
		GLint count;

		GLint size;
		GLenum type;

		const GLsizei bufSize = 16;
		GLchar name[bufSize];
		GLsizei length;
		std::vector<std::string> names;

		glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++) {
			glGetActiveUniform(id, (GLuint)i, bufSize, &length, &size, &type, name);
			names.push_back(name);
		}

		return names;
	}
}