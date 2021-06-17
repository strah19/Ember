#ifndef OPENGL_VERTEX_ARRAY_H
#define OPENGL_VERTEX_ARRAY_H

#include "OpenGLBuffers.h"

namespace EmberGL {
	enum class VertexBufferFormat {
		VNCVNCVNC,
		VVVCCCNNN
	};

	class VertexArray {
	public:
		VertexArray();
		virtual ~VertexArray();

		void Bind();
		void UnBind();

		std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() { return vertex_buffers; };
		uint32_t GetIndexBufferSize() const { return index_size; }

		void AddVertexBuffer(VertexBuffer* vertex_buf, VertexBufferFormat format);
		void SetIndexBufferSize(uint32_t index_buf) { index_size = index_buf; }

		void EnableVertexAttrib(uint32_t index);
		void SetArrayForInstancing(std::shared_ptr<VertexBuffer>& vertex_buf, uint32_t offset_sizes[], uint32_t stride_sizes[]);

		uint32_t GetId() const { return vertex_array_buffer_id; }
	private:
		uint32_t vertex_array_buffer_id;

		std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers;

		uint32_t index_size = 0;
	};
}

#endif // !OPENGL_VERTEX_ARRAY_H
