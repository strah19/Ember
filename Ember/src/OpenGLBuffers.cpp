#include "OpenGLBuffers.h"
#include <glad/glad.h>

namespace EmberGL {
	static uint32_t current_index_buffer_id = 0;
	static uint32_t current_vertex_buffer_id = 0;
	static uint32_t current_uniform_buffer_id = 0;

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		glGenBuffers(1, &vertex_buffer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(uint32_t size) {
		glGenBuffers(1, &vertex_buffer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &vertex_buffer_id);
	}

	void VertexBuffer::Bind() {
		if (current_vertex_buffer_id != vertex_buffer_id) {
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
			current_index_buffer_id = vertex_buffer_id;
		}
	}

	void VertexBuffer::UnBind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	void VertexBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t size) {
		glGenBuffers(1, &index_buffer_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		count = size / sizeof(*indices);
	}

	IndexBuffer::IndexBuffer(uint32_t size) {
		glGenBuffers(1, &index_buffer_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		count = 0;
	}

	void IndexBuffer::SetData(uint32_t* data, uint32_t size) {
		Bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
		count = size / sizeof(*data);
	}

	IndexBuffer::~IndexBuffer() {
		glDeleteBuffers(1, &index_buffer_id);
	}

	void IndexBuffer::Bind() {
		if (current_index_buffer_id != index_buffer_id) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
			current_index_buffer_id = index_buffer_id;
		}
	}

	void IndexBuffer::UnBind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		current_index_buffer_id = 0;
	}

	static uint32_t uniform_buffer_point_latest = 0;
	UniformBuffer::UniformBuffer(uint32_t size) {
		glGenBuffers(1, &uniform_buffer_id);
		uniform_buffer_point = uniform_buffer_point_latest++;
		Bind();
		AllocateData(size);
		size_of_buffer = size;
	}

	UniformBuffer::~UniformBuffer() {
		glDeleteBuffers(1, &uniform_buffer_id);
	}

	void UniformBuffer::Bind() {
		if (current_uniform_buffer_id != uniform_buffer_id) {
			glBindBuffer(GL_UNIFORM_BUFFER, uniform_buffer_id);
			current_uniform_buffer_id = uniform_buffer_id;
		}
	}

	void UniformBuffer::UnBind() {
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	uint32_t UniformBuffer::GetId() const {
		return uniform_buffer_id;
	}

	void UniformBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	}

	uint32_t UniformBuffer::GetUniformBlockId(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void UniformBuffer::BindToShader(uint32_t shader_id, const std::string& block_name) {
		glUniformBlockBinding(shader_id, GetUniformBlockId(shader_id, block_name), uniform_buffer_point);
		BindToBindPoint();
	}

	void UniformBuffer::BindToBindPoint() {
		glBindBufferRange(GL_UNIFORM_BUFFER, uniform_buffer_point, uniform_buffer_id, 0, size_of_buffer);
	}

	void UniformBuffer::AllocateData(uint32_t size) {
		glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	static uint32_t current_indirect_draw_buffer = 0;
	IndirectDrawBuffer::IndirectDrawBuffer(uint32_t size) {
		glGenBuffers(1, &indirect_buffer_id);
		Bind();
		AllocateData(size, nullptr);
		size_of_buffer = size;
	}

	IndirectDrawBuffer::~IndirectDrawBuffer() {
		glDeleteBuffers(1, &indirect_buffer_id);
	}

	void IndirectDrawBuffer::Bind() {
		if (current_indirect_draw_buffer != indirect_buffer_id) {
			glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer_id);
			current_indirect_draw_buffer = indirect_buffer_id;
		}
	}

	void IndirectDrawBuffer::UnBind() {
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	}

	uint32_t IndirectDrawBuffer::GetId() const {
		return indirect_buffer_id;
	}

	void IndirectDrawBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		Bind();
		glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
	}

	void IndirectDrawBuffer::AllocateData(uint32_t size, void* data) {
		glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, GL_DYNAMIC_DRAW);
	}

	static uint32_t shader_buffer_point_latest = 0;
	static uint32_t current_shader_storage_id = 0;
	ShaderStorageBuffer::ShaderStorageBuffer(uint32_t size) {
		glGenBuffers(1, &shader_storage_id);
		Bind();
		AllocateData(size, nullptr);
		size_of_buffer = size;
		binding_point = shader_buffer_point_latest++;
	}

	ShaderStorageBuffer::~ShaderStorageBuffer() {
		glDeleteBuffers(1, &shader_storage_id);
	}

	void ShaderStorageBuffer::Bind() {
		if (current_shader_storage_id != shader_storage_id) {
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, shader_storage_id);
			current_shader_storage_id = shader_storage_id;
		}
	}

	void ShaderStorageBuffer::UnBind() {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	uint32_t ShaderStorageBuffer::GetId() const {
		return shader_storage_id;
	}

	void ShaderStorageBuffer::SetData(void* data, uint32_t size, uint32_t offset) {
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	}

	void ShaderStorageBuffer::AllocateData(uint32_t size, void* data) {
		glBufferData(GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); 
	}

	uint32_t ShaderStorageBuffer::GetUniformBlockId(uint32_t shader_id, const std::string& block_name) {
		return glGetUniformBlockIndex(shader_id, block_name.c_str());
	}

	void ShaderStorageBuffer::BindToShader(uint32_t shader_id, const std::string& block_name) {
		glShaderStorageBlockBinding(shader_id, GetUniformBlockId(shader_id, block_name), binding_point);
		BindToBindPoint();
	}

	void ShaderStorageBuffer::BindToBindPoint() {
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point, shader_storage_id);
	}
}