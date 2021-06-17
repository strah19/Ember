#ifndef OPENGL_RENDERER_COMMANDS_H
#define OPENGL_RENDERER_COMMANDS_H

#include <memory>
#include "OpenGLVertexArray.h"

namespace EmberGL {
	class RendererAPI {
	public:
		void Init();
		void Clear();
		void SetClearColor(float r, float g, float b, float a);
		void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		void DrawVertexArray(std::shared_ptr<VertexArray> vertex_array);
		void DrawVertexArrayInstanced(std::shared_ptr<VertexArray> vertex_array, uint32_t instance_count);
		void DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride);
	};
}

#endif // !OPENGL_RENDERER_COMMANDS_H
