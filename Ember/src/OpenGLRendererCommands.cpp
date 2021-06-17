#include "OpenGLRendererCommands.h"

namespace EmberGL {
	static std::shared_ptr<RendererAPI> renderer_api;

	void RendererCommand::Init() {
		renderer_api = std::make_shared<RendererAPI>();
		renderer_api->Init();
	}

	void RendererCommand::SetViewport(uint32_t x, uint32_t y, uint32_t w, uint32_t h) { 
		renderer_api->SetViewport(x, y, w, h); 	
	}
	void RendererCommand::Clear() { 
		renderer_api->Clear(); 
	}

	void RendererCommand::SetClearColor(float r, float g, float b, float a) { 
		renderer_api->SetClearColor(r, g, b, a);
	}

	void RendererCommand::DrawVertexArray(std::shared_ptr<VertexArray> vertex_array) {
		renderer_api->DrawVertexArray(vertex_array);
	}

	void RendererCommand::DrawVertexArrayInstanced(std::shared_ptr<VertexArray> vertex_array, uint32_t instance_count) {
		renderer_api->DrawVertexArrayInstanced(vertex_array, instance_count);
	}

	void RendererCommand::DrawMultiIndirect(const void* indirect, uint32_t count, uint32_t stride) {
		renderer_api->DrawMultiIndirect(indirect, count, stride);
	}
}