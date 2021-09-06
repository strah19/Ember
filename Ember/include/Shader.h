#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include <memory>
#include <glm.hpp>
#include <unordered_map>

namespace Ember {
	using ShaderSources = std::unordered_map<uint32_t, std::stringstream>;

	class Shader {
	public:
		Shader(const std::string& file_path);
		Shader() = default;

		virtual ~Shader();

		void Bind();
		void UnBind();

		void Init(const std::string& file_path);

		/* Uniforms go here! */
		void Set1f(const std::string& name, float value);
		void SetMat4f(const std::string& name, const glm::mat4& mat4);
		void SetVec3f(const std::string& name, const glm::vec3& vec3);
		void SetIntArray(const std::string& name, int* array, uint32_t size);

		uint32_t GetUniformLocation(const std::string& name);
		uint32_t GetId() const { return shader_id; }
	private:
		uint32_t shader_id;
		ShaderSources ParseShader(const std::string& file_path);
		uint32_t CompileShader(const std::string& source, uint32_t type);
		uint32_t CreateShader(const ShaderSources& shader_sources);
	};
}

#endif // !OPENGL_SHADER_H
