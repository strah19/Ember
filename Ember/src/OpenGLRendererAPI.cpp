#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace EmberGL {
	void RendererAPI::Init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_MULTISAMPLE);
	}

	void RendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		glViewport(x, y, w, h);
	}

	void RendererAPI::DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) {
		glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	void RendererAPI::DrawVertexArrayInstanced(std::shared_ptr<VertexArray> vertex_array, uint32_t instance_count) {
		glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_array->GetIndexBufferSize(), instance_count);
	}

	void RendererAPI::DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride) {
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, indirect, count, stride);
	}
}