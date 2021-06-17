#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace EmberGL {
	static uint32_t current_vertex_array_id = 0;

	static GLenum VertexShaderTypeToOpenGL(VertexShaderType type) {
		switch (type) {
		case VertexShaderType::Float: return GL_FLOAT;
		case VertexShaderType::Int: return GL_INT;
		case VertexShaderType::None: return GL_NONE;
		}
		return GL_NONE;
	}

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &vertex_array_buffer_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &vertex_array_buffer_id);
	}

	void VertexArray::Bind(){
		if (current_vertex_array_id != vertex_array_buffer_id) {
			glBindVertexArray(vertex_array_buffer_id);
			current_vertex_array_id = vertex_array_buffer_id;
		}
	}

	void VertexArray::UnBind(){
		glBindVertexArray(0);
		current_vertex_array_id = 0;
	}

	void VertexArray::AddVertexBuffer(VertexBuffer* vertex_buf, VertexBufferFormat format) {
		uint32_t stride = vertex_buf->GetLayout()->Calculate();

		vertex_buf->Bind();
		Bind();

		for (auto& elements : vertex_buf->GetLayout()->GetLayout()) {
			switch (format) {
			case VertexBufferFormat::VNCVNCVNC:
				glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
					stride * GetSizeInBytes(elements.type),
					(void*)(elements.offset * GetSizeInBytes(elements.type)));
				break;
			case VertexBufferFormat::VVVCCCNNN:
				glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
					0,
					(void*)(elements.offset * GetSizeInBytes(elements.type)));
				break;
			}

			EnableVertexAttrib(elements.index);
		}
	}

	void VertexArray::EnableVertexAttrib(uint32_t index) {
		glEnableVertexAttribArray(index);
	}

	void VertexArray::SetArrayForInstancing(std::shared_ptr<VertexBuffer>& vertex_buf, uint32_t offset_sizes[], uint32_t stride_sizes[]) {
		vertex_buf->Bind();
		Bind();
		uint32_t i = 0;
		for (auto& elements : vertex_buf->GetLayout()->GetLayout()) {
			glVertexAttribPointer(elements.index, elements.size, VertexShaderTypeToOpenGL(elements.type), elements.normalized ? GL_TRUE : GL_FALSE,
				stride_sizes[i],
				(void*)offset_sizes[i]);
			i++;
			EnableVertexAttrib(elements.index);
		}
	}
}