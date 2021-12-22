#include "RendererCommands.h"
#include <glad/glad.h>

namespace Ember {
	static int prim;

	void RendererCommand::Init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glEnable(GL_MULTISAMPLE);

		prim = TRIANGLE;
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
		glViewport(x, y, w, h);
	}
	void RendererCommand::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererCommand::SetClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void RendererCommand::DrawVertexArray(VertexArray* vertex_array) {
		glDrawElements(DecodeType(), vertex_array->GetIndexBufferSize(), GL_UNSIGNED_INT, 0);
	}

	void RendererCommand::DrawVertexArrayInstanced(VertexArray* vertex_array, uint32_t instance_count) {
		glDrawArraysInstanced(DecodeType(), 0, vertex_array->GetIndexBufferSize(), instance_count);
	}

	void RendererCommand::DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride) {
		glMultiDrawElementsIndirect(DecodeType(), GL_UNSIGNED_INT, indirect, count, stride);
	}

	void RendererCommand::PolygonMode(uint32_t face, uint32_t mode) {
		glPolygonMode(face, mode);
	}

	void RendererCommand::SetPrimType(int prim_type) {
		prim = prim_type;
	}

	int RendererCommand::DecodeType() {
		switch (prim) {
		case TRIANGLE: return GL_TRIANGLES;
		case LINE: return GL_LINES;
		case POINT: return GL_POINTS;
		default: return GL_TRIANGLES;
		}
	}

	void RendererCommand::LineWidth(float width) {
		glLineWidth(width);
	}
}