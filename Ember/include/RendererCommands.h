#ifndef RENDERER_COMMANDS_H
#define RENDERER_COMMANDS_H

#include "VertexArray.h"

namespace Ember {
	class RendererCommand {
	public:
		static void Init();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
		static void Clear();
		static void SetClearColor(float r, float g, float b, float a);
		static void DrawVertexArray(VertexArray* vertex_array);
		static void DrawVertexArrayInstanced(VertexArray* vertex_array, uint32_t instance_count);
		static void DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride);
		static void PolygonMode(uint32_t face, uint32_t mode);
	};
}

#endif // !RENDERER_COMMANDS_H